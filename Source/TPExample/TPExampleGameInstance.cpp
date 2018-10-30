// Fill out your copyright notice in the Description page of Project Settings.

#include "TPExampleGameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

UTPExampleGameInstance::UTPExampleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/TPExample/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != NULL)) return;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/TPExample/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != NULL)) return;

	MenuClass = MenuBPClass.Class;
	InGameMenuClass = InGameMenuBPClass.Class;
}

void UTPExampleGameInstance::Init()
{
	IOnlineSubsystem* OnlineSubsystemInterface = IOnlineSubsystem::Get();
	if (OnlineSubsystemInterface != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found OSS: %s"), *OnlineSubsystemInterface->GetSubsystemName().ToString());

		OnlineSessionInterface = OnlineSubsystemInterface->GetSessionInterface();
		if (OnlineSessionInterface.IsValid())
		{
			OnlineSessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UTPExampleGameInstance::OnCreateSessionComplete);
			OnlineSessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UTPExampleGameInstance::OnDestroySessionComplete);
			OnlineSessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UTPExampleGameInstance::OnFindSessionsComplete);
			OnlineSessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UTPExampleGameInstance::OnJoinSessionComplete);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Found no OSS"));
}

void UTPExampleGameInstance::LoadMainMenu()
{
	if (!ensure(MenuClass != NULL)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != NULL)) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UTPExampleGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass != NULL)) return;

	UMenuWidget* InGameMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(InGameMenu != NULL)) return;

	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);
}


// 
// From IMenuInterfaces
//
void UTPExampleGameInstance::Host()
{
	if (OnlineSessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = OnlineSessionInterface->GetNamedSession("My Session");

		if (ExistingSession != NULL)
		{
			// Destroy the session if it exists before creating another one
			OnlineSessionInterface->DestroySession("My Session");
		}
		else
		{
			// Create a session if there's no existing one
			FOnlineSessionSettings SessionSettings;
			SessionSettings.bIsLANMatch = false;
			SessionSettings.NumPublicConnections = 2;
			SessionSettings.bShouldAdvertise = true;
			SessionSettings.bUsesPresence = true;

			OnlineSessionInterface->CreateSession(0, "My Session", SessionSettings);
		}
	}
}

void UTPExampleGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session creating not successful"));
		return;
	}
	if (Menu != NULL)
	{
		Menu->Teardown();
		UE_LOG(LogTemp, Warning, TEXT("Menu Teared dwon"));
	}

	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) return;

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UTPExampleGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session detroying not successful"));
		return;
	}
	// Create a session if there's no existing one
	FOnlineSessionSettings SessionSettings;
	OnlineSessionInterface->CreateSession(0, "My Session", SessionSettings);
}

void UTPExampleGameInstance::OnFindSessionsComplete(bool bWasSussessful)
{
	if (bWasSussessful && SessionSearch.IsValid() && Menu != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Num of sessions found: %d"), OnlineSessionInterface->GetNumSessions());

		TArray<FString> FoundSessionNames;
		for (const FOnlineSessionSearchResult& Result : SessionSearch->SearchResults)
		{
			FString SessionName = Result.GetSessionIdStr();
			FoundSessionNames.AddUnique(SessionName);
			UE_LOG(LogTemp, Warning, TEXT("Session Id: %s"), *Result.GetSessionIdStr());
		}
		Menu->SetServerList(FoundSessionNames);
	}
}

void UTPExampleGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!OnlineSessionInterface.IsValid()) return;

	FString ConnectInfo;
	if (!OnlineSessionInterface->GetResolvedConnectString(SessionName, ConnectInfo)) return;

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	PlayerController->ClientTravel(ConnectInfo, ETravelType::TRAVEL_Absolute);
}

void UTPExampleGameInstance::Join(uint32 Index)
{
	if (!OnlineSessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != NULL)
	{
		Menu->Teardown();
		UE_LOG(LogTemp, Warning, TEXT("Menu Teared down"));
	}

	OnlineSessionInterface->JoinSession(0, "My Session", SessionSearch->SearchResults[Index]);
}

void UTPExampleGameInstance::Refresh()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		OnlineSessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UTPExampleGameInstance::QuitToMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	PlayerController->ClientTravel("/Game/TPExample/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UTPExampleGameInstance::QuitGameInstance()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	PlayerController->ConsoleCommand(TEXT("quit"));
}