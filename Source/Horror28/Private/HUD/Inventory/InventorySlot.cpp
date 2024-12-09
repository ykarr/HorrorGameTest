// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventorySlot.h"
/*UI Component*/
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
/*ItemData*/
#include "Actor/ItemBaseActor.h"
#include "Actor/ItemData/ItemDataTable.h"
/*Inventory Menu*/
#include "HUD/Inventory/InventoryMenu.h"
#include "Level1/L1HGHUD.h"

void UInventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ItemRef = nullptr;
	SlotButton->SetIsEnabled(false);
	SlotImage->SetBrushFromTexture(ItemIcon);
	SetQuantityHide();
	SlotButton->OnReleased.AddDynamic(this, &UInventorySlot::SlotButtonReleased);
	SlotButton->OnHovered.AddDynamic(this, &UInventorySlot::SlotButtonHovered);
	SlotButton->OnUnhovered.AddDynamic(this, &UInventorySlot::SlotButtonUnHovered);
}
void UInventorySlot::UpdateInventorySlot(AItemBaseActor* Item)
{
	if (Item) {
		ItemRef = Item;
		SlotButton->SetIsEnabled(true);
		FItemData* ItemDataTable = Item->GetItemData();
		SlotImage->SetBrushFromTexture(ItemDataTable->ItemIcon);
		//FNumberFormattingOptions NumberFormat;
		//NumberFormat.UseGrouping = true;
		if (Item->ItemQuantity > 1) {
			ItemQuantity->SetText(FText::AsNumber(Item->ItemQuantity, &FNumberFormattingOptions().SetUseGrouping(true)));
			SetQuantityVisible();
		}
		else {
			SetQuantityHide();
		}
	}
	else {
		ItemRef = nullptr;
		SlotButton->SetIsEnabled(false);
		SlotImage->SetBrushFromTexture(ItemIcon);
		SetQuantityHide();
		//InventoryMenuRef->HideDropDownBtn();
	}
}

void UInventorySlot::SetRowCol(int32 row, int32 col)
{
	Row = row;
	Col = col;
}

void UInventorySlot::SlotButtonReleased()
{
	/*if (!InventoryMenuRef) {
		AHGCharacter* HGCharacter = Cast<AHGCharacter>(GetOwningPlayerPawn());
		InventoryMenuRef = HGCharacter->GetHUD()->InventoryMain;
	}*/
	UE_LOG(LogTemp, Display, TEXT("buttonReleased"));
	if (InventoryMenuRef) {
		FVector2D temp = GetDesiredSize()+20;
		int32 DropDownX = GetCol() + 0;
		int32 DropDonwY = GetRow() + 1;
		FVector2D Loc = FVector2D(DropDownX*temp.X+10, DropDonwY*temp.Y-10);
		//UE_LOG(LogTemp, Display, TEXT("loc: %d %d"), DropDownX, DropDonwY);
		InventoryMenuRef->OnDropDownMenu(Loc,index);
	}
}

void UInventorySlot::SlotButtonHovered()
{
	InventoryMenuRef->ShowItemDescription(ItemRef);
}

void UInventorySlot::SlotButtonUnHovered()
{
	InventoryMenuRef->ShowItemDescription(nullptr);
}

void UInventorySlot::SetQuantityHide()
{
	ItemQuantity->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventorySlot::SetQuantityVisible()
{
	ItemQuantity->SetVisibility(ESlateVisibility::Visible);
}

