// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UButton;
class UImage;
class UTextBlock;
struct FItemData;
class AItemBaseActor;
class UInventoryMenu;
class UInventoryGrid;
/**
 * 
 */
UCLASS()
class HORROR28_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//FItemData* ItemData;
	//UInventoryGrid* InventoryGridRef;
	UInventoryMenu* InventoryMenuRef;
	void UpdateInventorySlot(AItemBaseActor* Item);
	void SetRowCol(int32 row, int32 col);
	//void UpdateInventorySlot(AItemBaseActor* Item, int32 Quantity);
	UFUNCTION()
	void SlotButtonReleased();
	UFUNCTION()
	void SlotButtonHovered();
	UFUNCTION()
	void SlotButtonUnHovered();
	int32 index;
	//AL1HGHUD* HUD;
	//TObjectPtr< UInventoryDropDown> DropDown;
	//UPROPERTY(EditAnywhere)
	//TSubclassOf< UInventoryDropDown> DropDownClass;
protected:
	virtual void NativeOnInitialized() override;
	void SetQuantityVisible();
	void SetQuantityHide();

	UPROPERTY(meta = (BindWidget))
	UButton* SlotButton;
	UPROPERTY(meta = (BindWidget))
	UImage* SlotImage;
	//UPROPERTY(EditAnywhere)
	UPROPERTY(EditAnywhere)
	int32 ItemQuantity_;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemQuantity;
	UPROPERTY(EditAnywhere)
	UTexture2D* ItemIcon;

	int32 Row;
	int32 Col;

	AItemBaseActor* ItemRef;
public:
	FORCEINLINE int32 GetRow() const { return Row; }
	FORCEINLINE int32 GetCol() const { return Col; }
	
};
