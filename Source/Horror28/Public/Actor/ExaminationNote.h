// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Examination.h"
#include "ExaminationNote.generated.h"

class UTextRenderComponent;

/**
 * 
 */
UCLASS()
class HORROR28_API AExaminationNote : public AExamination
{
	GENERATED_BODY()

public:
	AExaminationNote();
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	TObjectPtr <UTextRenderComponent> TextRender;
	UPROPERTY(EditAnywhere)
	TObjectPtr <UTextRenderComponent> BackTextRender;
	UPROPERTY(EditAnywhere)
	FText NoteText;
	UPROPERTY(EditAnywhere)
	FText BackNoteText;


};
