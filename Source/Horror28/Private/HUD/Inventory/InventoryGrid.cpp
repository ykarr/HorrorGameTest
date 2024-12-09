// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryGrid.h"
#include "Character/HGCharacter.h"
/*inventory*/
#include "Components/UniformGridPanel.h"
#include "Character/InventoryComponent.h"
#include "HUD/Inventory/InventorySlot.h"

#include "Components/UniformGridSlot.h"
#include "HUD/Inventory/InventoryMenu.h"
#include "Level1/L1HGHUD.h"
void UInventoryGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UInventoryGrid::NativeConstruct()
{
	Super::NativeConstruct();
	
	//UE_LOG(LogTemp, Display, TEXT("UInventoryGrid : NativeConstruct"));

	//UE_LOG(LogTemp, Display, TEXT("UInventoryGrid : NativeOnInitialized"));
	//InventoryGridPanel->
	if (GetOwningPlayerPawn()) {
		HGCharacter = Cast<AHGCharacter>(GetOwningPlayerPawn());
		if (HGCharacter) {
			InventoryRef = HGCharacter->GetInventory();
			InventoryRef->InventoryGridref = this;
			InventorySlotCapacity = InventoryRef->GetInventorySlotCapacity();
			//HUD = HGCharacter->GetHUD();
			InventoryMenuRef->InventoryGrid = this;
			InventoryMenuRef->InventoryComponentRef = InventoryRef;
		}
		if (InventorySlotClass) {
			for (int32 i = 0; i < InventorySlotCapacity; i++) {
				TObjectPtr<UInventorySlot> ItemSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
				ItemSlot->index = i;
				UUniformGridSlot* GridSlot = InventoryGridPanel->AddChildToUniformGrid(ItemSlot, i / 4, i % 4);
				//GridSlot = InventoryGridPanel->AddChildToUniformGrid(ItemSlot, i / 4, i % 4);
				ItemSlot->InventoryMenuRef = InventoryMenuRef;
				ItemSlot->SetRowCol(GridSlot->GetRow(), GridSlot->GetColumn());
				SlotsArray.Add(ItemSlot);
				//GridSlotsArray.Add(GridSlot);
			}

		}
	}
	
}

void UInventoryGrid::CustomInit()
{
	
}

void UInventoryGrid::AddItemSlots(int32 Amount)
{
	//GEngine->AddOnScreenDebugMessage(4, 1, FColor::Green, FString(TEXT("AddSlots")), false);
	for (int32 i = 0; i < Amount; i++) {
		UE_LOG(LogTemp, Display, TEXT("Inven :%d"), InventorySlotCapacity + i);
		TObjectPtr<UInventorySlot> ItemSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		ItemSlot->index = InventorySlotCapacity + i;
		UUniformGridSlot* GridSlot = InventoryGridPanel->AddChildToUniformGrid(ItemSlot, (InventorySlotCapacity + i) / 4, (InventorySlotCapacity + i) % 4);
		//GridSlot = InventoryGridPanel->AddChildToUniformGrid(ItemSlot, InventorySlotCapacity+i / 4, InventorySlotCapacity+i % 4);
		ItemSlot->InventoryMenuRef = InventoryMenuRef;
		ItemSlot->SetRowCol(GridSlot->GetRow(), GridSlot->GetColumn());
		SlotsArray.Add(ItemSlot);
		//GridSlotsArray.Add(GridSlot);
	}
	InventorySlotCapacity += Amount;
}

TObjectPtr<UInventorySlot> UInventoryGrid::GetInventorySlot(int32 index)
{
	return SlotsArray[index];
}
