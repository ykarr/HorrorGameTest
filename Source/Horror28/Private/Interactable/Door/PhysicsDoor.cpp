// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Door/PhysicsDoor.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/SceneComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
// Sets default values
APhysicsDoor::APhysicsDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>("Root");
	SceneRoot->SetupAttachment(RootComponent);
	SetRootComponent(SceneRoot);
	DoorframeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Doorframe"));
	DoorframeMesh->SetupAttachment(RootComponent);
	//SetRootComponent(DoorframeMesh);
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorMesh->SetupAttachment(DoorframeMesh);
	DoorMesh->SetRelativeLocation(FVector(0,45.f,0));
	DoorMesh->SetSimulatePhysics(true);
	//DoorMesh->SetMassOverrideInKg(NAME_None, 150.f);	//MassInKg=150으로 설정.
	DoorMesh->SetAngularDamping(4.f);
	
	PhysicsConstraint = CreateDefaultSubobject< UPhysicsConstraintComponent>(TEXT("PhysicsConstraint"));
	PhysicsConstraint->SetupAttachment(RootComponent);
	PhysicsConstraint->SetRelativeLocation(FVector(0, 45.f, 0));
	PhysicsConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 120.f);
	PhysicsConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
	PhysicsConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
	PhysicsConstraint->SetDisableCollision(true);
	
	//Constraint이름설정은 Blueprint에서
	//PhysicsConstraint->SetConstrainedComponents(Component1, FName("Doorframe"), Component2, FName("Door"));
	//PhysicsConstraint->SetConstrainedComponents(Component1, DoorframeMesh.GetFName(), Component2,DoorframeMesh.GetFName());
}

// Called when the game starts or when spawned
void APhysicsDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicsDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APhysicsDoor::GrabObject(AHGCharacter* Character)
{
	HGCharacter = Character;
	//UCharacterMovementComponent* Movement=Character->GetCharacterMovement();
	Character->GetCharacterMovement()->DisableMovement();
	Character->GetController()->SetIgnoreLookInput(true);
	//UE_LOG(LogTemp, Display, TEXT("asdfasdfas"));
	//Character->GetController()->EnableInput(PlayerController);
	//GetWorld()->GetFirstPlayerController()->GetPawn()->DisableInput(nullptr);
	//if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	//{
	//	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	//{
	//	//	// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
	//	//	Subsystem->AddMappingContext(DoorContext, 1);
	//	//}

	//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	//	{
	//		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APhysicsDoor::Look);
	//	}
	//}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APhysicsDoor::Look);
	}
}

void APhysicsDoor::ReleaseObject()
{
	
	HGCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	HGCharacter->GetController()->ResetIgnoreLookInput();
	//GetWorld()->GetFirstPlayerController()->GetPawn()->EnableInput(nullptr);
	//if (APlayerController* PlayerController = Cast<APlayerController>(HGCharacter->GetController()))
	//{
	//	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	//	//{
	//	//	Subsystem->RemoveMappingContext(DoorContext);
	//	//}
	//	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
	//	{
	//		EnhancedInputComponent->ClearBindingsForObject(this);
	//	}
	//}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
	}
	HGCharacter = nullptr;
}



void APhysicsDoor::Look(const FInputActionValue& Value)
{
	FVector2D value = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Display, TEXT(""));
	MoveDoor(value.X);
}

void APhysicsDoor::MoveDoor(float Value)
{
	FVector ALocation=GetActorLocation() - HGCharacter->GetActorLocation();
	FVector FwdVector = GetActorForwardVector();
	float DotProductResult = FVector::DotProduct(ALocation.GetSafeNormal(), FwdVector);
	FRotator DoorRotator(0, Value, 0);
	FRotator CombinedRotator;
	if(DotProductResult>0){
		CombinedRotator = (DoorRotator*1*0.5)+DoorMesh->GetRelativeRotation();
	}
	else {
		CombinedRotator = (DoorRotator * -1 * 0.5) + DoorMesh->GetRelativeRotation();
	}
	
	//CombinedRotator = (DoorRotator * 1 * 0.5) + DoorMesh->GetRelativeRotation();
	DoorMesh->SetRelativeRotation(CombinedRotator);
}

