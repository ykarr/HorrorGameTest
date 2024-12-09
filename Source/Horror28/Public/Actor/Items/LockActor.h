// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "LockActor.generated.h"

class ALockDial;
class USkeletalMeshComponent;
class UCameraComponent;
class UBoxComponent;
class UCharacterMovementComponent;
class AHGCharacter;
class UInputAction;
class UWidgetInteractionComponent;
UCLASS()
class HORROR28_API ALockActor : public AActor,public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	//UFUNCTION(BlueprintCallable)
	bool bCheckCode();
	void OpenAnim();
	UPROPERTY(EditAnywhere, Category = "Mesh")
	USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> LockMesh;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<UBoxComponent> LockBoxC;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	TSubclassOf<ALockDial> LockDialClass;
	UPROPERTY(BlueprintReadWrite)
	TArray<TObjectPtr<ALockDial>> LockDialArray;
	int32 LockCombination[4];

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* ViewCamera;
	UPROPERTY(EditAnywhere, Category = "BoxSetting")
	FVector BoxPos;
	UPROPERTY(EditAnywhere, Category = "BoxSetting")
	FVector BoxExtent;
	bool bIsLockOpen;
protected:
	ALockActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	virtual void Interact() override;
	void DelayStartFun();
	void EnableActionInput();
	void EscapeLockView();
	void EnableLeftMouseBtn();
	void DisableLeftMouseBtn();
	TObjectPtr<UCharacterMovementComponent> PlayerMovement;
	TObjectPtr<AHGCharacter> HGCharacter;

	UPROPERTY(EditAnywhere, Category = "input")
	TObjectPtr<UInputAction> EscapeAction;
	UPROPERTY(EditAnywhere, Category = "input")
	TObjectPtr<UInputAction> leftMouseBtn;
	//UPROPERTY(EditAnywhere, Category = "input")
	//TObjectPtr<UInputAction> InteractAction;
	FTimerHandle DelayHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	UWidgetInteractionComponent* WidgetInteractionComponent;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimationAsset> OpenAnimation;
};
