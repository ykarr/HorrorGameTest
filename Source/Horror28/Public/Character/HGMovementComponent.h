// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimeLineComponent.h"

#include "HGMovementComponent.generated.h"

class AHGCharacter;
class UCharacterMovementComponent;
class UHGWalkCameraShake;
class UHGRunCameraShake;
class UAttributeProgressWidget;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORROR28_API UHGMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	void InitalCharacterState();
	void StopSprint();
	void HGCharacterSprint();
	void HGCharacterCrouching();
	float GetStaminaBarPercent();
	void SetStaminaWidget();
	void OnLanding();
	void LeanRight();
	void UnLeanRight();
	void LeanLeft();
	void UnLeanLeft();
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchHalfHeight;
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bIsCrouching;
	TObjectPtr<UAttributeProgressWidget> AttributeProgressWidget;
protected:
	UHGMovementComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	TSubclassOf<UHGWalkCameraShake> HGWalkCameraShakeClass;
	TSubclassOf<UHGRunCameraShake> HGRunCameraShakeClass;

	TObjectPtr<AHGCharacter> HGCharacter;
	TObjectPtr<UCharacterMovementComponent> PlayerMovement;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxWalkSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpZ;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float GroundSpeed;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxStamina;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float MinStamina;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float CurrentStamina;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchSpeed;
	/**/
	UPROPERTY(EditAnywhere, Category = "FootStepSound")
	TArray<TObjectPtr<USoundBase>> FootStepSoundArray;
	//UPROPERTY(EditAnywhere, Category = "FootStepSound", meta = (AllowPrivateAccess="true"))

	/*Timeline*/
	FTimeline LerpTimeline;
	FTimeline FootStepTimeline;
	FTimeline RightLeanTimeline;
	FTimeline LeftLeanTimeline;

	/*Curve Data*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* Curve1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* FootStepCurve1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* LeanCurve;

	/*TimerHandle*/
	FTimerHandle SprintTimerHandle;
	FTimerHandle RegenStaminaTimerHandle;
	FTimerHandle RegenCooltime;
	//FTimerHandle FootSteptimerHandle;
	
	UFUNCTION()
	void FloatTemp();

	UFUNCTION()
	void ShortenPlayerCapsule(float Value);
	void SprintTimer();
	void RegenStamina();
	UFUNCTION()
	void RightUpdateLean(float Value);
	UFUNCTION()
	void LeftUpdateLean(float Value);
	UFUNCTION()
	void PlayFootStepSound();
public:	

	FORCEINLINE void SetHGCharacter(AHGCharacter* Character) { HGCharacter= Character; }
		
};
