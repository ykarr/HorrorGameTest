// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "HGCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class IInteractInterface;
class IGrabInterface;
class UInventoryComponent;
class AL1HGHUD;

class UInputAction;
class UInputMappingContext;

class UAttributeComponent;
class UHGMovementComponent;
class UFlashLightComponent;
class AEnemyBase;
class USpotLightComponent;

UCLASS()
class HORROR28_API AHGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	bool bIsCanOpenInven;
	bool bIsHiding;
	UPROPERTY(EditAnywhere)
	float LookSensitivity;
	/*Character Component*/
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* ViewCamera;
	UPROPERTY(EditAnywhere,Category="Light")
	USpringArmComponent* SprintArm;

	UPROPERTY(EditAnywhere, Category = "Light")
	UFlashLightComponent* FlashLightComponent;
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	UHGMovementComponent* HGMovementComponent;
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeComponent> AttributeComponent;

	void TestText();
	UFUNCTION(BlueprintCallable)
	void CaughByEnemy(APawn* Enemy);
	//void RotateToEnemy();
protected:
	AHGCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	
	/*Character Component*/
	UPROPERTY(EditAnywhere, Category = "Inventory")
	UInventoryComponent* InventoryComponent;
	

	/*Character Input Context & InputAction*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr< UInputMappingContext> HGContext;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> LMButtonAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> FlashLightAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> SprintAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> InventoryAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> LeanLeftAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	TObjectPtr<UInputAction> LeanRightAction;

private:

	FTimerHandle EnemyRotDelay;
	UPROPERTY(EditAnywhere)
	USpotLightComponent* FlashLight;
	AActor* GrabableObject;
	/*Interface*/
	UPROPERTY(VisibleAnywhere, Category = "Interface")
	TScriptInterface<IInteractInterface> InteractInterface;
	UPROPERTY(VisibleAnywhere, Category= "Interface")
	TScriptInterface<IGrabInterface> GrabInterface;
	void InitPos();
	/*Inital HUD*/
	void InitHUD();
	/*InputBind Function*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SetFlashLight();
	void HGSprintStart();
	void HGSprintStop();
	void ToggleInventory();
	void HGCrouchStarted();
	void HGCrouchEnded();
	void LeanRightFunc();
	void UnLeanRightFunc();
	void LeanLeftFunc();
	void UnLeanLeftFunc();
	//Grab Interace Function
	void GrabCheck();
	void GrabReleaseCheck();
	//Interaction Interface Function
	void InteractableCheck();
	virtual void Landed(const FHitResult& Hit) override;
	//Line Trace Return Actor
	AActor* LineTrace();
	AHGCharacter* HGCharacter;
	TObjectPtr<AL1HGHUD> HUD;
	TObjectPtr<APlayerController> HGController;
	bool bIsPaused;
	bool bIsOnce;
public:
	FORCEINLINE AL1HGHUD* GetHUD() const { return HUD; }
	FORCEINLINE AHGCharacter* GetHGCharacter() const { return HGCharacter; }
	FORCEINLINE UInventoryComponent* GetInventory() const { return InventoryComponent; }
};
