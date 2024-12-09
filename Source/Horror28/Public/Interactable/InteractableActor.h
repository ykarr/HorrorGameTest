// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "InteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS()
class HORROR28_API AInteractableActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	virtual void Interact() override;
protected:
	AInteractableActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Interactable | Mesh")
	TObjectPtr<UStaticMeshComponent> InteractableMesh;


public:	

};
