// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Inventory/NoteExaminationWidget.h"
#include "Actor/Items/NoteBase.h"
#include "Actor/ExaminationNote.h"
#include <Blueprint\WidgetLayoutLibrary.h>
#include "Components/TextRenderComponent.h"
#include "EnhancedInputComponent.h"
void UNoteExaminationWidget::NativeConstruct()
{
	
}
void UNoteExaminationWidget::UpdateNoteExamin(ANoteBase* NoteBase)
{
	NoteBaseRef = NoteBase;
	


	//이동막기.
	GetWorld()->GetFirstPlayerController()->GetPawn()->DisableInput(nullptr);
	//마우스 막기.
	FInputModeGameAndUI InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	//FString NoteTextString = NoteBase->NoteText.ToString();
	//UE_LOG(LogTemp, Display, TEXT("NoteBase 이름: %s"), *NoteTextString);
	ExaminationNoteRef->TextRender->SetText(NoteBase->NoteText);
	ExaminationNoteRef->BackTextRender->SetText(NoteBase->BackNoteText);
	EnableExitInput();
}
void UNoteExaminationWidget::EnableExitInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent)) {
		EnhancedInputComponent->BindAction(ExitAction,ETriggerEvent::Triggered,this, &UNoteExaminationWidget::ExitBtnPress);
	}
}
void UNoteExaminationWidget::DisableExitInput()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent)) {
		EnhancedInputComponent->ClearBindingsForObject(this);
	}
	
}
void UNoteExaminationWidget::ExitBtnPress()
{

	GetWorld()->GetFirstPlayerController()->GetPawn()->EnableInput(nullptr);
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	DisableExitInput();
	RemoveFromParent();
}
void UNoteExaminationWidget::ResetRot()
{
	/*RotReset*/
	ExaminationNoteRef->ResetRot();
}
FReply UNoteExaminationWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//FReply Reply=Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		bIsRotateMesh = true;
		MouseLoc = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		return  FReply::Handled();
	}
	return  FReply::Unhandled();
}

FReply UNoteExaminationWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//FReply Reply = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (!InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton)) {
		bIsRotateMesh = false;
		return FReply::Handled();
	}

	return  FReply::Unhandled();
}

FReply UNoteExaminationWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	//FReply Reply = Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	if (bIsRotateMesh) {
		FVector2D tmp = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()) - MouseLoc;
		FRotator ExaminItemRotate(0, tmp.X * -1, tmp.Y * -1);
		ExaminationNoteRef->ItemMesh->AddWorldRotation(ExaminItemRotate*0.1);
		MouseLoc = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		return FReply::Handled();
	}
	return FReply::Unhandled();
}