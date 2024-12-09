// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PickupWidget.generated.h"

class UImage;
class UTexture2D;
/**
 * 
 */
UCLASS()
class HORROR28_API UPickupWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateInteractWidget();
	void TogglePrompt(bool bIsCanPickup);
	UPROPERTY(meta = (BindWidget))
	UImage* PromptImage;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* ArrowImage;
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* PickupBtnImage;
	
};
