// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerHideInterface.generated.h"
class AHideActor;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerHideInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HORROR28_API IPlayerHideInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void DidEnemySee(AHideActor* HideActor);
	virtual void LeftHidingSpot();
};
