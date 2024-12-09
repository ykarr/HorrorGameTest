// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AttributeProgressWidget.generated.h"

class UProgressBar;
class AHGCharacter;
/**
 * 
 */
UCLASS()
class HORROR28_API UAttributeProgressWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	AHGCharacter* HGCharacter;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> StaminaBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> BatteryBar;
	virtual void NativeConstruct()override;
	virtual void NativeOnInitialized()override;
	UFUNCTION()
	float SetBatteryPercent() const;
	UFUNCTION()
	float SetStaminaBarPercent() const;
	UFUNCTION()
	float SetHealthBarPercent() const;

};


/*
	//void GetHealthBarPercent();
	//void SetHealthBarPercent(float Percent);
	//void SetStaminaBarPercent(float Percent);

	//void SetBatteryPercent(float Percent);

*/