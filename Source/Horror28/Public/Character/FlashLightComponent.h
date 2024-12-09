// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlashLightComponent.generated.h"

class USpotLightComponent;
class UAttributeProgressWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORROR28_API UFlashLightComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	TObjectPtr<UAttributeProgressWidget> AttributeProgressWidget;
	TObjectPtr<USpotLightComponent> LightRef;
	/*BatteryState*/
	UPROPERTY(EditDefaultsOnly, Category = "BatteryState")
	float MaxBatteryLevel;
	UPROPERTY(EditDefaultsOnly, Category = "BatteryState")
	float CurrentBatteryLevel;
	UPROPERTY(EditDefaultsOnly, Category = "BatteryState")
	float MinBatteryLevel;
	UPROPERTY(EditDefaultsOnly, Category = "BatteryState")
	float DepleteAmount;
	float DepleteSpeed;
	void SetBatteryTimer(bool bIsToogle);
	void DepleteBatteryLife();
	void AddBatteryLife(float Amount);
	void SetIntensity();
	bool IsNotFullBattery();
	float GetBatteryBarPercent();
	void SetBatteryWidget();
protected:
	// Called when the game starts
	UFlashLightComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float Intensity;
	FTimerHandle DepleteBatteryTimerHandle;
public:	
	// Called every frame

		
};
