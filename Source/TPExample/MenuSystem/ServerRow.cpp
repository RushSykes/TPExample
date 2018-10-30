// Fill out your copyright notice in the Description page of Project Settings.

#include "ServerRow.h"

#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "MainMenu.h"


void UServerRow::Setup(UMainMenu* Parent, uint32 Index)
{
	if (Parent != NULL)
	{
		this->Parent = Parent;
		this->Index = Index;
		ServerRowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Parent null"));
	}
}

void UServerRow::OnClicked()
{
	if (Parent != NULL)
	{
		Parent->SetSelectedIndex(this->Index);
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Parent null"));
}