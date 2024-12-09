// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/InventoryMenu.h"
#include "HUD/Inventory/InventoryGrid.h"
#include "HUD/Inventory/InventorySlot.h"
#include "HUD/Inventory/InventoryDropDown.h"
#include "HUD/Inventory/InventorySlot.h"
#include "Actor/ItemBaseActor.h"
#include "Character/InventoryComponent.h"
#include "Actor/ItemBaseActor.h"

/*UI Component*/
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
void UInventoryMenu::ShowItemDescription(AItemBaseActor* SelectItemBase)
{
	if (SelectItemBase) {
		FItemData* ItemData=SelectItemBase->GetItemData();

		ItemNameText->SetText(ItemData->ItemName);
		ItemDescriptionText->SetText(ItemData->ItemDescription);
		ItemInfoVerticalBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
	}
	else {
		ItemInfoVerticalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}
void UInventoryMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
}
void UInventoryMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InventoryGrid->InventoryMenuRef = this;
	
	//opDownTest = CreateWidget<UInventoryDropDown>(this, InventoryDropDownClass);
	//DropDownTest->SetVisibility(ESlateVisibility::Visible);
	HideDropDownBtn();

	//HideCloseDropBtn();
	CloseDropDown->OnClicked.AddDynamic(this, &UInventoryMenu::HideDropDownBtn);
	
}

void UInventoryMenu::OnDropDownMenu(FVector2D Loc,int32 index)
{
	AItemBaseActor* SelectItem = InventoryComponentRef->GetInventoryContents(index);
	FItemData* ItemData = SelectItem->GetItemData();
	if (!ItemData->bIsCanDropable && !ItemData->bIsCanExamination && !ItemData->bIsCanUseable) {
		return;
	}
	DropDown->SetRenderTranslation(Loc);
	DropDown->UpdateMenu(SelectItem);
	DropDown->index = index;
	DisplayDropDownBtn();
	//ShowItemDescription(SelectItem);
	//DisplayCloseDropBtn();
}

//void UInventoryMenu::HideCloseDropBtn()
//{
//	CloseDropDown->SetVisibility(ESlateVisibility::Collapsed);
//}
//void UInventoryMenu::DisplayCloseDropBtn()
//{
//	CloseDropDown->SetVisibility(ESlateVisibility::Visible);
//}
void UInventoryMenu::HideDropDownBtn()
{
	CloseDropDown->SetVisibility(ESlateVisibility::Collapsed);
	DropDown->SetVisibility(ESlateVisibility::Collapsed);
	ShowItemDescription(nullptr);
}
void UInventoryMenu::DisplayDropDownBtn()
{
	CloseDropDown->SetVisibility(ESlateVisibility::Visible);
	DropDown->SetVisibility(ESlateVisibility::Visible);
}


