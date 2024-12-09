// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "EnemyAnimInstance.generated.h"

class AHGCharacter;
class AEnemyBase;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class HORROR28_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FRotator HeadRotation;
	virtual void NativeInitializeAnimation()override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;
	AHGCharacter* HGCharacter;
	TObjectPtr<AEnemyBase> EnemyRef;
	TObjectPtr<UCharacterMovementComponent> EnemyMovement;
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool Isfalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;
};
