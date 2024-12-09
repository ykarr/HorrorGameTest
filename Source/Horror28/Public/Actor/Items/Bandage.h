// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/ItemBaseActor.h"
#include "Bandage.generated.h"

/**
 * 
 */
UCLASS()
class HORROR28_API ABandage : public AItemBaseActor
{
	GENERATED_BODY()
protected:
	ABandage();
	virtual void UseItem() override;
	float AddHealthAmount;
};
