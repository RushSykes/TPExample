// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenu.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"

#include "ServerRow.h"


UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/TPExample/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != NULL)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (!ensure(HostButton != NULL)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(JoinButton != NULL)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelJoinButton != NULL)) return false;
	CancelJoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinSubButton != NULL)) return false;
	JoinSubButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!ensure(QuitButton != NULL)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	return true;
}

void UMainMenu::HostServer()
{
	if (MenuInterface != NULL)
		MenuInterface->Host();
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != NULL)) return;
	if (!ensure(JoinMenu != NULL)) return;

	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (MenuInterface != NULL)
		MenuInterface->Refresh();
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != NULL)) return;
	if (!ensure(MainMenu != NULL)) return;

	MenuSwitcher->SetActiveWidget(MainMenu);
}

void UMainMenu::SetServerList(TArray<FString> ServerNames)
{
	if (!ensure(ServerList != NULL)) return;

	ServerList->ClearChildren();

	uint32 Index = 0;
	for (const FString& ServerName : ServerNames)
	{
		UServerRow* TempRow = CreateWidget<UServerRow>(this, ServerRowClass);
		if (!ensure(TempRow != NULL)) return;

		TempRow->ServerName->SetText(FText::FromString(ServerName));
		TempRow->Setup(this, Index++);
		ServerList->AddChild(TempRow);
	}
}

void UMainMenu::SetSelectedIndex(uint32 Index)
{
	SelectedRowIndex = Index;
}

void UMainMenu::JoinServer()
{
	if (SelectedRowIndex.IsSet() && MenuInterface != NULL)
	{
		uint32 Index = SelectedRowIndex.GetValue();
		UE_LOG(LogTemp, Warning, TEXT("Selected row index: %d"), Index);
		//if (!ensure(JoinTextBox != NULL)) return;
		//const FString& Address = JoinTextBox->GetText().ToString();
		MenuInterface->Join(Index);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected row index not set."));
	}
}

void UMainMenu::QuitGame()
{
	if (MenuInterface != NULL)
		MenuInterface->QuitGameInstance();
}