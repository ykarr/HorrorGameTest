// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GrabInterface.h"
#include "InputActionValue.h"
#include "PhysicsDoor.generated.h"

class UPhysicsConstraintComponent;
class UPrimitiveComponent;
class UInputAction; 
class UInputMappingContext;

UCLASS()
class HORROR28_API APhysicsDoor : public AActor,public IGrabInterface
{
	GENERATED_BODY()
	
public:	

protected:
	APhysicsDoor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> DoorframeMesh;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* PhysicsConstraint;
	UPROPERTY(EditAnywhere, Category = "Root")
	USceneComponent* SceneRoot;
	//UPrimitiveComponent* Component1;
	//UPrimitiveComponent* Component2;
private:
	//APlayerController* PlayerController;
	AHGCharacter* HGCharacter;
public:	
	virtual void GrabObject(AHGCharacter* Character)override;
	virtual void ReleaseObject()override;
	void Look(const FInputActionValue& Value);
	void MoveDoor(float Value);
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr< UInputMappingContext> DoorContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> LookAction;
};
