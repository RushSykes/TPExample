// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class TPEXAMPLE_API UServerRow : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	bool bIsSelected;
	void Setup(class UMainMenu* Parent, uint32 Index);
	void SetColor(FLinearColor InColorAndOpacity);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ServerRowButton;

	UPROPERTY()
	class UMainMenu* Parent;

	UFUNCTION()
	void OnClicked();

	UFUNCTION()
	void OnHovered();

	UFUNCTION()
	void OnUnHoverd();
	
	uint32 Index;
};
