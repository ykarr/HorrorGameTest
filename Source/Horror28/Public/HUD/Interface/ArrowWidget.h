// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArrowWidget.generated.h"

class UButton;
class ALockDial;
/**
 * 
 */
UCLASS()
class HORROR28_API UArrowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ArrowBtn;
	virtual void NativeOnInitialized()override;
	UFUNCTION()
	void OnUpPressBtn();
	bool bIsAddRot;
	TObjectPtr<ALockDial> LockDialRef;
};
