// Fill out your copyright notice in the Description page of Project Settings.

#include "InGameMenu.h"
#include <Components/Button.h>

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize())
		return false;

	if (!ensure(CancelButton != NULL)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::Resume);

	if (!ensure(QuitButton != NULL)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::Quit);

	return true;
}

void UInGameMenu::Resume()
{
	Teardown();
}

void UInGameMenu::Quit()
{
	// Quit to main menu
	if (MenuInterface != NULL)
	{
		Teardown();
		MenuInterface->QuitToMainMenu();
	}
}