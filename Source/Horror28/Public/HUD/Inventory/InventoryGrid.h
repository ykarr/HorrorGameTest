// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

class UUniformGridPanel;
class UInventoryComponent;
class AHGCharacter;
class UInventorySlot;
class UUniformGridSlot;
class UInventoryMenu;
/**
 * 
 */
UCLASS()
class HORROR28_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()
public:

	int32 InventorySlotCapacity;
	UInventoryMenu* InventoryMenuRef;

	void CustomInit();
	void AddItemSlots(int32 Amount);
protected:
	virtual void NativeConstruct()override; //이거다음
	virtual void NativeOnInitialized() override;//이거?
	
	TArray<TObjectPtr< UInventorySlot>> SlotsArray;
	UInventoryComponent* InventoryRef;
	TArray<TObjectPtr< UUniformGridSlot>> GridSlotsArray;
	AHGCharacter* HGCharacter;
	//AL1HGHUD* HUD;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> InventorySlotClass;
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* InventoryGridPanel;
	//UUniformGridSlot* GridSlot;
public:
	
	TObjectPtr< UInventorySlot> GetInventorySlot(int32 index);
	//FORCEINLINE UInventoryGrid* GetInventoryGrid() const { return InventoryGrid; };
	//TObjectPtr< UUniformGridSlot> GetInventoryGridSlot(int32 index) {
	//	return GridSlotsArray[index];
	//};
};
