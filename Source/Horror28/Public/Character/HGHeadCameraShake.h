// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LegacyCameraShake.h"
#include "HGHeadCameraShake.generated.h"

/**
 * LegacyCameraShake
 */
UCLASS()
class HORROR28_API UHGHeadCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void HorrorWalkCamera();
	UFUNCTION()
	void HorrorRunCamera();
protected:
	UHGHeadCameraShake();
};
