// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"
class AEnemyAIController;
UCLASS()
class HORROR28_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, Category = "SkeletonMesh")
	//USkeletalMeshComponent* SkeletonMeshComponent;
	FORCEINLINE FVector GetTargetPlayerLoc() const { return TargetLoc; };
protected:

	AEnemyBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	ACharacter* PlayerCharacter;
	FVector TargetLoc;
	TObjectPtr<AEnemyAIController> EnemyAI;


};
