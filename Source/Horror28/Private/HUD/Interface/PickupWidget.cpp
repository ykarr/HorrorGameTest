// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Interface/PickupWidget.h"
#include "Components/Image.h"

void UPickupWidget::UpdateInteractWidget()
{
}

void UPickupWidget::TogglePrompt(bool bIsCanPickup)
{
	if (bIsCanPickup) {
		PromptImage->SetBrushFromTexture(PickupBtnImage);
	}
	else {
		PromptImage->SetBrushFromTexture(ArrowImage);

	}
}


