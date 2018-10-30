// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"
#include "TPExampleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TPEXAMPLE_API UTPExampleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UTPExampleGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void Host() override;
	
	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	UFUNCTION()
	void Refresh() override;

	UFUNCTION()
	virtual void QuitToMainMenu() override;

	UFUNCTION()
	virtual void QuitGameInstance() override;
private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;
	IOnlineSessionPtr OnlineSessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSussessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
