// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Interface/AttributeProgressWidget.h"
#include "Components/ProgressBar.h"

#include "Character\HGCharacter.h"
#include "Character\FlashLightComponent.h"
#include "Character/HGMovementComponent.h"
#include "Character/AttributeComponent.h"
void UAttributeProgressWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}
void UAttributeProgressWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	HealthBar->PercentDelegate.BindUFunction(this, "SetHealthBarPercent");
	StaminaBar->PercentDelegate.BindUFunction(this, "SetStaminaBarPercent");
	BatteryBar->PercentDelegate.BindUFunction(this, "SetBatteryPercent");
}
float UAttributeProgressWidget::SetBatteryPercent() const
{
	//BatteryBar->SetPercent();
	//return (HGCharacter->FlashLightComponent->CurrentBatteryLevel - HGCharacter->FlashLightComponent->MinBatteryLevel) / HGCharacter->FlashLightComponent->MaxBatteryLevel;
	//UE_LOG(LogTemp, Display, TEXT("play"));
	return HGCharacter->FlashLightComponent->GetBatteryBarPercent();
}
float UAttributeProgressWidget::SetStaminaBarPercent() const
{
	return HGCharacter->HGMovementComponent->GetStaminaBarPercent();
}
float UAttributeProgressWidget::SetHealthBarPercent() const
{
	return HGCharacter->AttributeComponent->GetHealthBarPercent();
}

/*
void UAttributeProgressWidget::SetHealthBarPercent(float Percent)
{
	HealthBar->SetPercent(Percent);
}
void UAttributeProgressWidget::SetStaminaBarPercent(float Percent)
{
	StaminaBar->SetPercent(Percent);
}

void UAttributeProgressWidget::SetBatteryPercent(float Percent)
{
	BatteryBar->SetPercent(Percent);
}
*/