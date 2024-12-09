// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Light/LightSwitchActor.h"
#include "Components/SpotLightComponent.h"
ALightSwitchActor::ALightSwitchActor()
{
}

void ALightSwitchActor::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* FoundActor = FindObject<AActor>(nullptr, *LightName.ToString());
	if (FoundActor) {
		SelectedLight = FoundActor->FindComponentByClass<USpotLightComponent>();
		if (!SelectedLight) {
			UE_LOG(LogTemp, Warning, TEXT("Selected actor does not have a SpotLightComponent!"));
		}
	}
}

void ALightSwitchActor::Interact()
{
	Super::Interact();
	SelectedLight->ToggleVisibility();
}
