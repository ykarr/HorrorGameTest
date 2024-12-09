// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryMenu.generated.h"

class UInventoryGrid;
class UInventoryDropDown;
class UInventorySlot;
class UButton;
class UInventoryComponent;
class UVerticalBox;
class UTextBox;
class UTextBlock;
class AItemBaseActor;
/**
 * 
 */
UCLASS()
class HORROR28_API UInventoryMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	TObjectPtr<UInventoryComponent> InventoryComponentRef;
	/*Grid*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryGrid> InventoryGrid; 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryDropDown> DropDown;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseDropDown;
	UFUNCTION()
	void HideDropDownBtn();
	UFUNCTION()
	void DisplayDropDownBtn();
	void OnDropDownMenu(FVector2D Loc, int32 index);

	/*ItemDescription*/
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox>  ItemInfoVerticalBox;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemNameText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescriptionText;

	void ShowItemDescription(AItemBaseActor* SelectItemBase);
protected:
	//virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	//TObjectPtr< UInventoryMenu> InventoryMenuref;
	virtual void NativeOnInitialized() override;
	


	
	
};
