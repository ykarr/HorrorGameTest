// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ItemBaseActor.h"
#include "FlashLightBattery.generated.h"


/**
 * 
 */
UCLASS()
class HORROR28_API AFlashLightBattery : public AItemBaseActor
{
	GENERATED_BODY()
	
public:
	AFlashLightBattery();
	virtual void BeginPlay() override;
	virtual void UseItem() override;
	virtual void ItemPickUp() override;
private:
	float BatteryAmount;

};
