// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectivesWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class HORROR28_API UObjectivesWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TObjectPtr<UTextBlock> ObjectiveText;

	void UpdateObjectiveText(FText InText);
};
