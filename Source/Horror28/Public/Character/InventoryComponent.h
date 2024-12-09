// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


class AItemBaseActor;
class UInventoryGrid;
class UExaminationWidget;
class AExamination;
class UInputAction;

USTRUCT()
struct FInventoryItemData {

	GENERATED_USTRUCT_BODY()
	FInventoryItemData() :InventoryItemData(nullptr),
		InQuantity(1) {};
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AItemBaseActor> InventoryItemData;
	UPROPERTY(VisibleAnywhere)
	int32 InQuantity;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORROR28_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	bool AddNewItem(AItemBaseActor* Item, int32 ItemQauntity);
	void UseItem(int32 index);
	void UpdateItemSlot(int32 index, AItemBaseActor* Item);
	void DropItem(int32 index);
	void CloseExamination();
	void HideExamination();
	int32 GetSlotIndex(AItemBaseActor* Item)const;
	AItemBaseActor* GetInventoryContents(int32 i) const;
	void AddMoreSlots(int32 Amount);
	//FInventoryItemData GetItemToInventory(int32 index) { return InventoryItems[index]; };
	/*Examination*/
	void ExaminationWidgetUpdate(int32 index);
	void CreateExaminationWidget();
	UPROPERTY(EditAnywhere, Category = "Examination")
	TSubclassOf<UExaminationWidget> ExaminationWidgetClass;
	UPROPERTY(EditAnywhere, Category = "Examination")
	TSubclassOf<AExamination> ExaminationClass;
	TObjectPtr<UExaminationWidget> ExaminationWidget;
	TObjectPtr<AExamination> ExaminationRef;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> ReturnAction;
	UInventoryGrid* InventoryGridref;
protected:
	UInventoryComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	int32 InventorySlotsCapacity;
private:
	int32 FindCheckEmptySlot(AItemBaseActor* Item);
	int32 FindNextPartialSlot(AItemBaseActor* Item);
	//UInventoryComponent* InventoryRef;
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<AItemBaseActor>> InventoryContents;
	UPROPERTY(EditAnywhere)
	TArray<FInventoryItemData> InventoryItems;
	//TArray<TPair< FInventoryItemData, int32>> items;
	bool HandleStackableItem(AItemBaseActor* Item, int32 ItemQauntity);
	bool HandleNonStackableItem(AItemBaseActor* Item, int32 ItemQauntity);
	int32 CalculateNumberForFullStack(AItemBaseActor* Item, int32 ItemQauntity);
public:	
	FORCEINLINE int32 GetInventorySlotCapacity() const { return InventorySlotsCapacity; }
	//FORCEINLINE UInventoryComponent 
};
