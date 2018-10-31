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
		this->bIsSelected = false;

		ServerRowButton->OnClicked.AddDynamic(this, &UServerRow::OnClicked);
		ServerRowButton->OnHovered.AddDynamic(this, &UServerRow::OnHovered);
		ServerRowButton->OnUnhovered.AddDynamic(this, &UServerRow::OnUnHoverd);
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
		// Green
		this->SetColorAndOpacity(FLinearColor(0.0f, 1.0f, 0.25f, 1.0f));
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Parent null"));
}

void UServerRow::OnHovered()
{
	// Yellow
	if(!bIsSelected)
		this->SetColor(FLinearColor(1.0f, 1.0f, 0.0f, 1.0f));
}

void UServerRow::OnUnHoverd()
{
	// White
	if(!bIsSelected)
		this->SetColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
}

void UServerRow::SetColor(FLinearColor InColorAndOpacity)
{
	ServerRowButton->SetColorAndOpacity(InColorAndOpacity);
}