// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/ExaminationWidget.h"
#include "Character/InventoryComponent.h"
#include "Actor/ItemBaseActor.h"
#include "Actor/Examination.h"
#include "Components/TextBlock.h"
//#include "EnhancedInputComponent.h"
#include "Character/HGCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
void UExaminationWidget::UpdateWidget(int32 Index)
{
	if (Index < 0) {
		
	}
	else {
		AItemBaseActor* ItemBaseActor = InventoryComponentRef->GetInventoryContents(Index);
		FItemData* ItemData = ItemBaseActor->GetItemData();
		AExamination* Examination = InventoryComponentRef->ExaminationRef;
		Examination->ItemMesh->SetStaticMesh(ItemData->ExaminationMesh);
		Mesh = Examination->ItemMesh;
		FVector CurrentLocation = Examination->ItemMesh->GetRelativeLocation();
		Examination->ItemMesh->SetRelativeLocation(FVector(ItemData->ExaminationMeshOffset + CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z));
		Examination->ItemMesh->SetRelativeRotation(ItemData->ExaminationMeshRotateOffset);
		ItemNameText->SetText(ItemData->ItemName);
		ItemDescriptionText->SetText(ItemData->ItemDescription);
	}
	
}

void UExaminationWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsRotateMesh = false;
	/*if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
		EnhancedInputComponent->BindAction(ReturnAction, ETriggerEvent::Triggered, this, &UExaminationWidget::StartReturnFunc);
	}*/
}

/*void UExaminationWidget::StartReturnFunc()
{
	GEngine->AddOnScreenDebugMessage(7, 1, FColor::Red, FString("Return"), true);
	InventoryComponentRef->CloseExamination();
}*/



FReply UExaminationWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//FReply Reply=Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		bIsRotateMesh = true;
		MouseLoc = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		return  FReply::Handled();
	}
	return  FReply::Unhandled();
}

FReply UExaminationWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		bIsRotateMesh = false;
		return FReply::Handled();
	}
	
	return  FReply::Unhandled();
	//bIsRotateMesh = false;
	//return FReply::Handled();
}

FReply UExaminationWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//FReply Reply = Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	if (bIsRotateMesh) {
		FVector2D tmp = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - MouseLoc;
		FRotator ExaminItemRotate(0,  tmp.X * -1, tmp.Y*-1);
		Mesh->AddWorldRotation(ExaminItemRotate);
		MouseLoc = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
