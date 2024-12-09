// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HGCharacter.h"
/*Input*/
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
/*Interface*/
#include "Interactable/InteractableActor.h"
#include "Interface/GrabInterface.h"
/*CharacterComponent*/
#include "Character/FlashLightComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/SpotLightComponent.h"
#include "Character/HGMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Character/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/AttributeComponent.h"
/*HUD*/
#include "Level1/L1HGHUD.h"
#include "HUD/MainWidget.h"
#include "HUD/Interface/AttributeProgressWidget.h"
#include "Level1/Ll1_GameStateBase.h"

//#include "Enemy/EnemyBase.h"
#include "Kismet\KismetMathLibrary.h"
#include "Enemy/EnemyAIController.h"

// Sets default values
AHGCharacter::AHGCharacter() :bIsCanOpenInven(true), bIsHiding(false), LookSensitivity(1), HGCharacter(this), bIsPaused(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(RootComponent);
	ViewCamera->SetRelativeLocation(FVector(0.f, 0.f, 60.f)); // Position the camera
	ViewCamera->bUsePawnControlRotation = true;

	/*FlashLight Arm*/
	SprintArm = CreateDefaultSubobject< USpringArmComponent>(TEXT("SprintArm"));
	SprintArm->SetupAttachment(ViewCamera);
	SprintArm->TargetArmLength=10.0f;
	SprintArm->SocketOffset=FVector(0, 0, -30);
	SprintArm->bEnableCameraRotationLag = true; //카메라 회전 지연을 활성화 또는 비활성화하는 부울 변수

	/*Light*/
	FlashLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLight"));
	FlashLight->SetupAttachment(SprintArm);
	FlashLightComponent = CreateDefaultSubobject<UFlashLightComponent>(TEXT("FlashLightComponent"));
	FlashLightComponent->LightRef = FlashLight;

	HGMovementComponent = CreateDefaultSubobject< UHGMovementComponent>(TEXT("HGMovementComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	AttributeComponent = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComponent"));

	bIsOnce = true;
}

// Called when the game starts or when spawned
void AHGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitPos();

	HGMovementComponent->SetHGCharacter(this);
	HGMovementComponent->InitalCharacterState();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			subsystem->AddMappingContext(HGContext, 0);
	}
	InitHUD();

}
void AHGCharacter::TestText()
{
	UE_LOG(LogTemp, Display, TEXT("Success"));
}
void AHGCharacter::CaughByEnemy(APawn* Enemy)
{
	GetCharacterMovement()->DisableMovement();
	Controller->DisableInput(nullptr);
	Controller->SetIgnoreLookInput(true);
	//FRotator Rott = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Enemy->GetActorLocation());
	//Controller->SetControlRotation(Rott);
	//if (Rott == Controller->GetControlRotation()) {
	//	GetWorld()->GetTimerManager().ClearTimer(EnemyRotDelay);
	//	if (bIsOnce) {
	//		bIsOnce = false;
	//		UE_LOG(LogTemp, Display, TEXT("CaughByEnemyCaughByEnemyCaughByEnemy"));
	//		//Cast<AEnemyAIController>(Enemy)->AttakPlayerMontage();
	//	}
	//}
	//bIsOnce = true;
	GetWorld()->GetTimerManager().SetTimer(EnemyRotDelay, [this, Enemy]() {
		FRotator Rott = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Enemy->GetActorLocation());
		FRotator Rottt = Controller->GetControlRotation();
		FRotator Rot = UKismetMathLibrary::RInterpTo(Rottt, Rott, GetWorld()->GetDeltaSeconds(), 10);
		Controller->SetControlRotation(Rot);
		Rottt = Controller->GetControlRotation();

		if (UKismetMathLibrary::EqualEqual_RotatorRotator(Rott, Rottt, 1)) {
			GetWorld()->GetTimerManager().ClearTimer(EnemyRotDelay);
			if (bIsOnce) {
				//UE_LOG(LogTemp, Display, TEXT("CaughByEnemyCaughByEnemyCaughByEnemy"));
				AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(Enemy->Controller);//->AttakPlayerMontage();
				EnemyAIController->AttakPlayerMontage();
				bIsOnce = false;
			}
		}
		else {
			//UE_LOG(LogTemp, Display, TEXT("NONONO"));
		}
		}, 0.01, true);
	bIsOnce = true;

}
void AHGCharacter::InitPos()
{
	FVector PlayerFirstVec = this->GetActorLocation();
	FVector PlayerSecondVec= this->GetActorLocation();
	PlayerSecondVec.Z += 3000;
	SetActorLocation(PlayerSecondVec);
	SetActorLocation(PlayerFirstVec);
}

void AHGCharacter::InitHUD()
{
	if (HGController = Cast<APlayerController>(GetController())) {
		HUD = Cast<AL1HGHUD>(HGController->GetHUD());
		HUD->MainWidget->AttributeProgressWidget->HGCharacter = this;
		AttributeComponent->AttributeProgressWidget= HUD->MainWidget->AttributeProgressWidget;
		HGMovementComponent->AttributeProgressWidget = HUD->MainWidget->AttributeProgressWidget;
		FlashLightComponent->AttributeProgressWidget =HUD->MainWidget->AttributeProgressWidget;
		ALl1_GameStateBase* GameStateBase = Cast<ALl1_GameStateBase>(GetWorld()->GetGameState());
		GameStateBase->Initialize(HUD->MainWidget);
		
		//GetWorld()->GetGameState()->
	}	
}
void AHGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHGCharacter::Move(const FInputActionValue& Value)
{
	FVector2D Movement = Value.Get<FVector2D>();
	if (Controller != nullptr) {
		//UE_LOG(LogTemp, Display, TEXT("moving"));
		AddMovementInput(GetActorForwardVector(),Movement.Y);
		AddMovementInput(GetActorRightVector(), Movement.X);
	}
}

void AHGCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();
	if (Controller != nullptr) {
	
		AddControllerYawInput(LookVector.X* LookSensitivity);
		AddControllerPitchInput(LookVector.Y* LookSensitivity);
	}
}



//Player Character Spinrt Start
void AHGCharacter::HGSprintStart()
{
	HGMovementComponent->HGCharacterSprint();
}
//Player Character Sprint End
void AHGCharacter::HGSprintStop()
{
	HGMovementComponent->StopSprint();
}

void AHGCharacter::ToggleInventory()
{
	if (bIsCanOpenInven) {
		if (bIsPaused) {
			bIsPaused = false;
			const FInputModeGameOnly InputMode;
			HGController->SetInputMode(InputMode);
			HGController->SetShowMouseCursor(false);
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			Controller->ResetIgnoreLookInput();
			HUD->HideInventory();
			InventoryComponent->HideExamination();
		}
		else {
			bIsPaused = true;
			const FInputModeGameAndUI InputMode;
			HGController->SetInputMode(InputMode);
			HGController->SetShowMouseCursor(true);
			GetCharacterMovement()->DisableMovement();
			Controller->SetIgnoreLookInput(true);
			HUD->DisplayInventory();
		}
	}
	
}

AActor* AHGCharacter::LineTrace()
{
	float Length=350;
	FVector Start= ViewCamera->GetComponentLocation();
	FVector Fwd = ViewCamera->GetForwardVector();
	FVector End = (Fwd * Length) + Start;
	FHitResult HitResult;
	DrawDebugLine(GetWorld(),Start,End,FColor::Red,false,1);
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility)) {
		//if (HitResult.PhysMaterial.IsValid()) {
		//	UE_LOG(LogTemp, Display, TEXT("Success"));
		//}
		return HitResult.GetActor();
	}
	return nullptr;
}
void AHGCharacter::HGCrouchStarted()
{
	UE_LOG(LogTemp, Display, TEXT("started"));
	HGMovementComponent->bIsCrouching = true;
	HGMovementComponent->HGCharacterCrouching();
}
void AHGCharacter::HGCrouchEnded()
{
	UE_LOG(LogTemp, Display, TEXT("Ended"));
	HGMovementComponent->bIsCrouching = false;
	HGMovementComponent->HGCharacterCrouching();
}
void AHGCharacter::LeanRightFunc()
{
	HGMovementComponent->LeanRight();
}
void AHGCharacter::UnLeanRightFunc()
{
	HGMovementComponent->UnLeanRight();
}
void AHGCharacter::LeanLeftFunc()
{
	HGMovementComponent->LeanLeft();
}

void AHGCharacter::UnLeanLeftFunc()
{
	HGMovementComponent->UnLeanLeft();
}

	
//Toggle FlashLight
void AHGCharacter::SetFlashLight()
{
	//FlashLight->ToggleVisibility();
	if (FlashLight->IsVisible()) { 
		UE_LOG(LogTemp, Display, TEXT("Notvisible"));
		FlashLightComponent->SetBatteryTimer(false);
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("visible"));
		FlashLightComponent->SetBatteryTimer(true);
	}
}

void AHGCharacter::InteractableCheck()
{
	//AttributeComponent->DebugDamage(true);
	if (AActor* TraceActor = LineTrace()) {
		if (TraceActor->GetClass()->ImplementsInterface(UInteractInterface::StaticClass())) {
			UE_LOG(LogTemp, Display, TEXT("hit"));
			InteractInterface = TraceActor;
			InteractInterface->Interact();
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Not hit"));
	}
}
void AHGCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	HGMovementComponent->OnLanding();
}
void AHGCharacter::GrabCheck()
{
	if (AActor* TraceActor = LineTrace()) {
		UE_LOG(LogTemp, Display, TEXT("hit"));
		if (TraceActor->GetClass()->ImplementsInterface(UGrabInterface::StaticClass())) {
			GrabInterface = TraceActor;
			GrabableObject = TraceActor;
			GrabInterface->GrabObject(this);
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Not hit"));
	}
}
void AHGCharacter::GrabReleaseCheck()
{
	if (GrabableObject) {
		UE_LOG(LogTemp, Display, TEXT("*GrabRelease*"));
		GrabableObject = nullptr;
		GrabInterface->ReleaseObject();
	}
}
// Called to bind functionality to input
void AHGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponen=CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponen->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponen->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponen->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHGCharacter::Move);
		EnhancedInputComponen->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHGCharacter::Look);
		EnhancedInputComponen->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AHGCharacter::InteractableCheck);
		EnhancedInputComponen->BindAction(LMButtonAction, ETriggerEvent::Started, this, &AHGCharacter::GrabCheck);
		EnhancedInputComponen->BindAction(LMButtonAction, ETriggerEvent::Completed, this, &AHGCharacter::GrabReleaseCheck);
		EnhancedInputComponen->BindAction(FlashLightAction, ETriggerEvent::Triggered, this, &AHGCharacter::SetFlashLight);
		EnhancedInputComponen->BindAction(SprintAction, ETriggerEvent::Started, this, &AHGCharacter::HGSprintStart);
		EnhancedInputComponen->BindAction(SprintAction, ETriggerEvent::Completed, this, &AHGCharacter::HGSprintStop);
		EnhancedInputComponen->BindAction(CrouchAction, ETriggerEvent::Started, this, &AHGCharacter::HGCrouchStarted);
		EnhancedInputComponen->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AHGCharacter::HGCrouchEnded);
		EnhancedInputComponen->BindAction(InventoryAction, ETriggerEvent::Started, this, &AHGCharacter::ToggleInventory);
		/*Lean*/
		//EnhancedInputComponen->BindAction(LeanRightAction, ETriggerEvent::Started, this, &AHGCharacter::LeanRightFunc);
		//EnhancedInputComponen->BindAction(LeanRightAction, ETriggerEvent::Completed, this, &AHGCharacter::UnLeanRightFunc);
		EnhancedInputComponen->BindAction(LeanLeftAction, ETriggerEvent::Started, this, &AHGCharacter::LeanLeftFunc);
		EnhancedInputComponen->BindAction(LeanLeftAction, ETriggerEvent::Completed, this, &AHGCharacter::UnLeanLeftFunc);
		//EnhancedInputComponen->BindAction(ReturnAction, ETriggerEvent::Triggered, this, &AHGCharacter::StartReturnFunc);
		//EnhancedInputComponen->BindAction(ReturnAction, ETriggerEvent::Triggered);
	}
}

