// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuWidget.h"

void UMenuWidget::SetMenuInterface(IMenuInterface* MenuInterface)
{
	this->MenuInterface = MenuInterface;
}

void UMenuWidget::Setup()
{
	this->AddToViewport();

	//Get player controller from UWorld and Set Input mode to UI only
	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

void UMenuWidget::Teardown()
{
	this->RemoveFromViewport();

	//Get player controller from UWorld and Set Input mode to Game only
	UWorld* World = GetWorld();
	if (!ensure(World != NULL)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != NULL)) return;

	FInputModeGameOnly InputModeData;
	//InputModeData.SetConsumeCaptureMouseDown(true);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;
}
