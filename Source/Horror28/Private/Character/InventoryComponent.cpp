// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/InventoryComponent.h"
#include "Actor/ItemBaseActor.h"
#include "HUD/Inventory/InventoryGrid.h"
#include "HUD/Inventory/InventorySlot.h"

#include "Components/UniformGridSlot.h"
#include "HUD/Inventory/InventoryMenu.h"
#include "HUD/Inventory/ExaminationWidget.h"
#include "Actor/Examination.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"

#include "Character/HGCharacter.h"
UInventoryComponent::UInventoryComponent():InventorySlotsCapacity(8)
{
	PrimaryComponentTick.bCanEverTick = true;
	InventoryItems.SetNum(InventorySlotsCapacity + 1);
	
}
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//InventoryContents.Init(nullptr, 10); // 크기가 10인 배열을 nullptr로 초기화
	InventoryContents.SetNum(10);
	ExaminationWidget = CreateWidget<UExaminationWidget>(GetWorld()->GetFirstPlayerController(),ExaminationWidgetClass);
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ExaminationClass, FoundActors);
	ExaminationWidget->InventoryComponentRef = this;
	ExaminationRef =Cast<AExamination>(FoundActors[0]);
	CreateExaminationWidget();
}
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::AddNewItem(AItemBaseActor* Item, int32 ItemQauntity)
{
	if (Item->bItemStackable) return HandleStackableItem(Item, ItemQauntity); //Stackable Item.
	else return HandleNonStackableItem(Item, ItemQauntity); //NonStackable Item.
	//return false;
}

void UInventoryComponent::UseItem(int32 index)
{
	if (IsValid(InventoryContents[index])) {
		//(LogTemp, Display, TEXT("holy"));
		AItemBaseActor* testItem =InventoryContents[index];
		testItem->UseItem();
	}
	else {
		//UE_LOG(LogTemp, Display, TEXT("Moly"));
		AItemBaseActor* Pickup = GetWorld()->SpawnActor<AItemBaseActor>(InventoryContents[index].GetClass());
		Pickup->UseItem();
	}

}

void UInventoryComponent::UpdateItemSlot(int32 index, AItemBaseActor* Item)
{
	InventoryContents[index] = Item;
	TObjectPtr<UInventorySlot> Slot = InventoryGridref->GetInventorySlot(index);
	Slot->UpdateInventorySlot(Item);
}

void UInventoryComponent::DropItem(int32 index)
{
	
	AItemBaseActor* DropableItem = InventoryContents[index];
	if (DropableItem->ItemQuantity > 0) {
		
		InventoryGridref->InventoryMenuRef->HideDropDownBtn();
		TObjectPtr<UInventorySlot> Slot = InventoryGridref->GetInventorySlot(index);
		if (DropableItem->ItemBaseActorClass) {
			AHGCharacter* player = Cast<AHGCharacter>(GetOwner());
			const FVector SpawnLoc{ GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 200)};
			const FTransform SpawnTransform{ GetOwner()->GetActorRotation(),SpawnLoc };
			AItemBaseActor* SpawnActor = GetWorld()->SpawnActor<AItemBaseActor>(DropableItem->ItemBaseActorClass, SpawnTransform);
			SpawnActor->ItemQuantity = DropableItem->ItemQuantity;
			//SpawnActor = DropableItem;
		}
		InventoryContents[index] =nullptr;
		Slot->UpdateInventorySlot(nullptr);

	}
}

void UInventoryComponent::CloseExamination()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
		//EnhancedInputComponent->BindAction(ReturnAction, ETriggerEvent::Triggered, this, &UExaminationWidget::StartReturnFunc);
	}
	HideExamination();
	InventoryGridref->InventoryMenuRef->SetVisibility(ESlateVisibility::Visible);
}

void UInventoryComponent::HideExamination()
{
	ExaminationWidget->SetVisibility(ESlateVisibility::Collapsed);
}

int32 UInventoryComponent::GetSlotIndex(AItemBaseActor* Item) const
{
	return InventoryContents.Find(Item);
}

bool UInventoryComponent::HandleStackableItem(AItemBaseActor* Item, int32 ItemQauntity)
{
	if (Item->MaxStackData >= ItemQauntity) {
		int32 AmountToDistribute = ItemQauntity;
		int32 StackItem = FindNextPartialSlot(Item);
		while (AmountToDistribute > 0 && StackItem>=0) {
			int32 InInvenItemQuantity = InventoryContents[StackItem]->ItemQuantity;
			//const int32 AmountMakeFullStack = InventoryContents[StackItem]->MaxStackData - InInvenItemQuantity;
			const int32 AmountMakeFullStack = CalculateNumberForFullStack(InventoryContents[StackItem],AmountToDistribute);
			AmountToDistribute -= AmountMakeFullStack;
			InventoryContents[StackItem]->ItemQuantity = AmountMakeFullStack+ InInvenItemQuantity;
			TObjectPtr<UInventorySlot> Slot = InventoryGridref->GetInventorySlot(StackItem);
			Slot->UpdateInventorySlot(InventoryContents[StackItem]);
			StackItem = FindNextPartialSlot(Item);
			if (AmountToDistribute <= 0) return true; //모든 아이템이 스택에 들어감.	
		}
		Item->ItemQuantity = AmountToDistribute;
		int32 EmptySlot = FindCheckEmptySlot(Item); //인벤토리에 스택이 없거나 기존 스택으로 해결안되는경우.
		if (EmptySlot != -1) {
			InventoryContents[EmptySlot] = Item;
			TObjectPtr<UInventorySlot> Slot = InventoryGridref->GetInventorySlot(EmptySlot);
			Slot->UpdateInventorySlot(Item);
			UE_LOG(LogTemp, Display, TEXT("Add Stackable"));
			return true;
		}
		UE_LOG(LogTemp, Display, TEXT("NotAdd"));
		return false;
	}
	return false;
}
bool UInventoryComponent::HandleNonStackableItem(AItemBaseActor* Item, int32 ItemQauntity)
{
		Item->ItemQuantity = 1;
		int32 EmptySlot = FindCheckEmptySlot(Item);
		if (EmptySlot != -1) {
			InventoryContents[EmptySlot] = Item;
			TObjectPtr< UInventorySlot> Slot = InventoryGridref->GetInventorySlot(EmptySlot);
			Slot->UpdateInventorySlot(Item);
			UE_LOG(LogTemp, Display, TEXT("Add Non Stackable"));
			return true;
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("NotAdd"));
			return false;
		}
}

int32 UInventoryComponent::CalculateNumberForFullStack(AItemBaseActor* Item, int32 ItemQauntity)
{
	const int32 AmountMakeFullStack = Item->MaxStackData - Item->ItemQuantity;
	return FMath::Min(ItemQauntity, AmountMakeFullStack);
}



AItemBaseActor* UInventoryComponent::GetInventoryContents(int32 i) const
{
	return InventoryContents[i];
}

void UInventoryComponent::AddMoreSlots(int32 Amount)
{
	InventorySlotsCapacity += Amount;
	InventoryItems.SetNum(InventorySlotsCapacity+1);
	InventoryGridref->AddItemSlots(Amount);
}

void UInventoryComponent::ExaminationWidgetUpdate(int32 index)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
		EnhancedInputComponent->BindAction(ReturnAction, ETriggerEvent::Triggered, this, &UInventoryComponent::CloseExamination);
	}
	ExaminationWidget->UpdateWidget(index);
	ExaminationWidget->SetVisibility(ESlateVisibility::Visible);
	//ExaminationWidget->AddToViewport(2);
	//ExaminationWidget->SetVisibility(ESlateVisibility::Collapsed);
	InventoryGridref->InventoryMenuRef->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryComponent::CreateExaminationWidget()
{
	ExaminationWidget->UpdateWidget(-1);
	ExaminationWidget->AddToViewport(2);
	HideExamination();
}



// Find Empty Slot, and if none are found return -1
int32 UInventoryComponent::FindCheckEmptySlot(AItemBaseActor* Item)
{
	for (int32 i = 0; i < InventorySlotsCapacity; i++) {
		if (!InventoryContents[i]) return i;
	}
	return -1;
}
//Return the index of Stackable Item slot, if no Valid index found return -1
int32 UInventoryComponent::FindNextPartialSlot(AItemBaseActor* Item)
{
	for (int32 i = 0; i < InventorySlotsCapacity; i++) {
		if (!InventoryContents[i]) return -1;
		if (InventoryContents[i]->ItemID == Item->ItemID && !InventoryContents[i]->IsFullItemStack()) return i;
	}
	return -1;
}


/*bool UInventoryComponent::HandleStackableItem(AItemBaseActor* Item, int32 ItemQauntity)
{
	if (Item->MaxStackData >= ItemQauntity) {
		FInventoryItemData NewItemData;
		NewItemData.InventoryItemData = Item;
		NewItemData.InQuantity = ItemQauntity;
		while (ItemQauntity <= 0) {
			int32 j = FindNextPartialSlot(Item);
			if (j >= 0) {

			}

		}
		int32 i = FindCheckEmptySlot(Item);
		if (i >= 0) {
			InventoryItems[i] = NewItemData;
			//InventoryContents.Insert(Item, i);
			InventoryContents[i] = Item;
			TObjectPtr< UInventorySlot> Slot = InventoryGridref->GetInventorySlot(i);
			Slot->UpdateInventorySlot(Item, ItemQauntity);
			UE_LOG(LogTemp, Display, TEXT("Add Stackable"));
			return true;
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("NotAdd"));
			return false;
		}
	}
}*/

/*
bool UInventoryComponent::HandleNonStackableItem(AItemBaseActor* Item, int32 ItemQauntity)
{
	FInventoryItemData NewItemData;
	NewItemData.InventoryItemData = Item;
	NewItemData.InQuantity = 1;
	int32 i = FindCheckEmptySlot(Item);
	if (i >= 0) {
		InventoryItems[i] = NewItemData;
		//InventoryContents.Insert(Item, i);
		InventoryContents[i] = Item;
		TObjectPtr< UInventorySlot> Slot = InventoryGridref->GetInventorySlot(i);
		Slot->UpdateInventorySlot(Item, 1);
		UE_LOG(LogTemp, Display, TEXT("Add Non Stackable"));
		return true;
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("NotAdd"));
		return false;
	}
}*/

/*
int32 UInventoryComponent::FindCheckEmptySlot(AItemBaseActor* Item)
{
	for (int32 i = 0; i < InventorySlotsCapacity; i++) {
		if (!InventoryItems[i].InventoryItemData) {
			// 현재 인덱스에 유효한 아이템이 없으면 빈 슬롯으로 간주하고 해당 인덱스를 반환합니다.
			UE_LOG(LogTemp, Display, TEXT("Inven: %d"),i);
			return i;
		}
		if (InventoryItems[i].InventoryItemData->ItemName == Item->ItemName) {
			UE_LOG(LogTemp, Display, TEXT("Inven: %d"), i);
		}
	}
	return -1;
}

//Stack가능한 아이템 Slot의 Index 반환. 유효한 인덱스가 없다면 -1반환.
int32 UInventoryComponent::FindNextPartialSlot(AItemBaseActor* Item)
{
	for (int32 i = 0; i < InventorySlotsCapacity; i++) {
		if (!InventoryItems[i].InventoryItemData) {
			// 현재 인덱스에 유효한 아이템이 없으면 빈 슬롯으로 간주하고 해당 인덱스를 반환합니다.
			return -1;
		}
		if (InventoryItems[i].InventoryItemData->ItemName == Item->ItemName) {
			UE_LOG(LogTemp, Display, TEXT("Inven: %d"), i);
			return i;
		}
	}
	return -1;
}
*/