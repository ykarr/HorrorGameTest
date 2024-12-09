// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NoteExaminationWidget.generated.h"

class AExaminationNote;
class UInputAction;
class ANoteBase;
/**
 * 
 */
UCLASS()
class HORROR28_API UNoteExaminationWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UNoteExaminationWidget();
	virtual void NativeConstruct()override;
	void UpdateNoteExamin(ANoteBase* NoteBase);
	void EnableExitInput();
	void DisableExitInput();
	void ExitBtnPress();
	void ResetRot();
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	TObjectPtr<AExaminationNote> ExaminationNoteRef;
	bool bIsRotateMesh;
	FVector2D MouseLoc;
	UStaticMeshComponent* Mesh;
	UPROPERTY(EditAnywhere)
	UInputAction* ExitAction;
	ANoteBase* NoteBaseRef;
};
