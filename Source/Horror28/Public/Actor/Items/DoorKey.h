// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ItemBaseActor.h"
#include "DoorKey.generated.h"

/**
 * 
 */
UCLASS()
class HORROR28_API ADoorKey : public AItemBaseActor
{
	GENERATED_BODY()
public:
	ADoorKey();
	virtual void BeginPlay() override;
	virtual void UseItem() override;
};
