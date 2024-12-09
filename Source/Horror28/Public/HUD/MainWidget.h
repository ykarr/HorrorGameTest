// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"
class UAttributeProgressWidget;
class UObjectivesWidget;
/**
 * Cross Hair?
 */
UCLASS()
class HORROR28_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UAttributeProgressWidget> AttributeProgressWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UObjectivesWidget> ObjectivesWidget;
};


