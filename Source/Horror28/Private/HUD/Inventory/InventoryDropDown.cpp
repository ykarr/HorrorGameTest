// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryDropDown.h"
#include "Actor/ItemBaseActor.h"
#include "Components/SizeBox.h"
#include "Components/Button.h"
#include "Character/HGCharacter.h"
#include "Character/InventoryComponent.h"

void UInventoryDropDown::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	UseButton->OnReleased.AddDynamic(this, &UInventoryDropDown::PressUseBtn);
	DropButton->OnReleased.AddDynamic(this, &UInventoryDropDown::DropBtn);
	ExamineButton->OnReleased.AddDynamic(this, &UInventoryDropDown::ExaminBtn);
}
void UInventoryDropDown::UpdateMenu(AItemBaseActor *ItemIn)
{
	//Get ItemSlot index. *
	//Get ItemData.
	//if can useable. useSection visible

	/* 이걸 visible 하기전에.*/
	FItemData* ItemData = ItemIn->GetItemData();
	if (ItemData->bIsCanUseable) {

		USESection->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		USESection->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ItemData->bIsCanExamination) {
		ExamineSection->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		ExamineSection->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (ItemData->bIsCanDropable) {
		DropSection->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		DropSection->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventoryDropDown::PressUseBtn()
{
	PlayerRef = Cast<AHGCharacter>(GetOwningPlayerPawn());
	UInventoryComponent * Inven=PlayerRef->GetInventory();
	Inven->UseItem(index);
}

void UInventoryDropDown::DropBtn()
{
	PlayerRef = Cast<AHGCharacter>(GetOwningPlayerPawn());
	UInventoryComponent* Inven = PlayerRef->GetInventory();
	Inven->DropItem(index);
}

void UInventoryDropDown::ExaminBtn()
{
	PlayerRef = Cast<AHGCharacter>(GetOwningPlayerPawn());
	UInventoryComponent* Inven = PlayerRef->GetInventory();
	Inven->ExaminationWidgetUpdate(index);
}
