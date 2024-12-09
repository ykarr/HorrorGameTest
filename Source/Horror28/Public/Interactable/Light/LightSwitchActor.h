// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "LightSwitchActor.generated.h"

class ULightComponentBase;
class USpotLightComponent;
class ALightActor;
/**
 * 
 */
UCLASS()
class HORROR28_API ALightSwitchActor : public AInteractableActor
{
	GENERATED_BODY()
public:
	ALightSwitchActor();
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "LightComponent")
	USpotLightComponent* SelectedLight;
	UPROPERTY(EditAnywhere, Category = "LightComponent")
	FName LightName;
	virtual void Interact() override;
};
