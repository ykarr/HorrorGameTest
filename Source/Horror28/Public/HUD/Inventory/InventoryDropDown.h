// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryDropDown.generated.h"

class UButton;
class USizeBox;
class AItemBaseActor;
class AHGCharacter;
/**
 * 
 */
UCLASS()
class HORROR28_API UInventoryDropDown : public UUserWidget
{
	GENERATED_BODY()
public:

	//virtual void NativeConstruct()override;
	virtual void NativeOnInitialized() override;
	UPROPERTY(meta = (BindWidget))
	USizeBox* USESection;
	UPROPERTY(meta = (BindWidget))
	USizeBox* ExamineSection;
	UPROPERTY(meta = (BindWidget))
	USizeBox* DropSection;

	UPROPERTY(meta = (BindWidget))
	UButton* UseButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExamineButton;
	UPROPERTY(meta = (BindWidget))
	UButton* DropButton;

	int32 index;
	AHGCharacter* PlayerRef;
	void UpdateMenu(AItemBaseActor* ItemIn);
	UFUNCTION()
	void PressUseBtn();
	UFUNCTION()
	void DropBtn();
	UFUNCTION()
	void ExaminBtn();
};
