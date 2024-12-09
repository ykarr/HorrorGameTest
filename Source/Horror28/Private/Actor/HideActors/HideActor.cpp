// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/HideActors/HideActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"

#include "Kismet\KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy/EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Character/HGCharacter.h"

// Sets default values
AHideActor::AHideActor():bIsCanInteract(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Locker = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Locker"));
	Locker->SetupAttachment(RootComponent);

	LockerDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockerDoor"));
	LockerDoor->SetupAttachment(Locker);
	LockerDoor->SetRelativeLocation(FVector(-37.5,39,0));
	

	PlayerPos = CreateDefaultSubobject<UArrowComponent>(TEXT("PlayerPosArrow"));
	PlayerPos->SetupAttachment(Locker);
	PlayerPos->SetRelativeLocation(FVector(0,0,88));
	PlayerPos->SetRelativeRotation(FRotator(0, 90, 0));

	ExitPos = CreateDefaultSubobject<UArrowComponent>(TEXT("ExitPosArrow"));
	ExitPos->SetupAttachment(Locker);
	ExitPos->SetRelativeLocation(FVector(0, 150, 88));
	ExitPos->SetRelativeRotation(FRotator(0, 90, 0));

	AILocation = CreateDefaultSubobject<UArrowComponent>(TEXT("AIPosArrow"));
	AILocation->SetupAttachment(Locker);
	AILocation->SetRelativeLocation(FVector(0, 150, 88));
	AILocation->SetRelativeRotation(FRotator(0, -90, 0));
}

// Called when the game starts or when spawned
void AHideActor::BeginPlay()
{
	Super::BeginPlay();
	HGCharacter = Cast<AHGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	OpenLockerSetting();
	CloseLockerSetting();
	OnFinishLocker.AddUObject(this, &AHideActor::DelayDoorClose);
	//if (HGCharacter) {
	//	GEngine->AddOnScreenDebugMessage(86, 10, FColor::Green, FString("GetGet"), true);
		
	//}
}

// Called every frame
void AHideActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LerpTimeline.TickTimeline(DeltaTime);
	CloseLerpTimeline.TickTimeline(DeltaTime);
	AIOpenLerpTimeline.TickTimeline(DeltaTime);
}

void AHideActor::Interact()
{
	if (bIsCanInteract) {
		HGCharacter->bIsCanOpenInven = false;
		bIsCanInteract = false;
		LerpTimeline.PlayFromStart();
	}

	AActor* EnemyActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyAIController::StaticClass());
	if (EnemyActor->GetClass()->ImplementsInterface(UPlayerHideInterface::StaticClass())) {
		UE_LOG(LogTemp, Display, TEXT("hit"));
		PlayerHideInterface = EnemyActor;
		PlayerHideInterface->DidEnemySee(this);
	}
	
}

void AHideActor::MoveCharacter()
{
	//FTransform Loc = UKismetMathLibrary::VInterpTo_Constant(HGCharacter->GetActorLocation(), PlayerPos->GetComponentLocation(), GetWorld()->DeltaTimeSeconds, 1000);
	//HGCharacter->SetActorTransform(Loc);
	
	FVector TargetLoc;
	FRotator TargetRot;
	if (HGCharacter->bIsHiding) {
		TargetLoc = ExitPos->GetComponentLocation();
		TargetRot = ExitPos->GetComponentRotation();
	}
	else {
		TargetLoc = PlayerPos->GetComponentLocation();
		TargetRot = PlayerPos->GetComponentRotation();

	}
	FVector Loc = UKismetMathLibrary::VInterpTo_Constant(HGCharacter->GetActorLocation(), TargetLoc, GetWorld()->GetDeltaSeconds(), 500);
	//FRotator NewRot = UKismetMathLibrary::FindLookAtRotation(HGCharacter->GetActorLocation(), TargetLoc);
	FTransform NewTransform;
	NewTransform.SetLocation(Loc);
	//NewTransform.SetRotation(FQuat(PlayerPos->GetComponentRotation()));
	HGCharacter->SetActorTransform(NewTransform);
	
	if (UKismetMathLibrary::Vector_Distance(HGCharacter->GetActorLocation(), TargetLoc)<=0.01) {
		//GEngine->AddOnScreenDebugMessage(85, 10, FColor::Green, FString("End"), true);
		GetWorld()->GetTimerManager().ClearTimer(CharacterMoveTimer);
		HGCharacter->Controller->SetControlRotation(TargetRot);
		FinishMoveCharacter();
		
	}
}

void AHideActor::FinishMoveCharacter()
{
	if (HGCharacter->bIsHiding) {
		HGCharacter->bIsCanOpenInven = true;
		AActor* EnemyActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyAIController::StaticClass());
		if (EnemyActor->GetClass()->ImplementsInterface(UPlayerHideInterface::StaticClass())) {
			UE_LOG(LogTemp, Display, TEXT("hit"));
			PlayerHideInterface = EnemyActor;
			PlayerHideInterface->LeftHidingSpot();
		}
		HGCharacter->SetActorEnableCollision(true);
		HGCharacter->bIsHiding = false;
		HGCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		
		
	}
	else {
		HGCharacter->bIsHiding = true;
		
	}
	OnFinishLocker.Broadcast();
}

void AHideActor::CloseLockerDoor()
{
	CloseLerpTimeline.ReverseFromEnd();
	bIsCanInteract = true;
}

void AHideActor::EnemyFound()
{
	if (bIsCanInteract) {
		LockerDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AIOpenLerpTimeline.PlayFromStart();
		
	}
}

void AHideActor::OpenLockerSetting()
{
	if (Curve1 != nullptr) {
		FOnTimelineFloat Curve1Callback;
		FOnTimelineEvent LerpTimeLineFinishCallBack;
		Curve1Callback.BindUFunction(this,FName("UpdateDoor"));
		LerpTimeLineFinishCallBack.BindUFunction(this, FName("DelayMove"));
		LerpTimeline.AddInterpFloat(Curve1, Curve1Callback);
		LerpTimeline.SetTimelineFinishedFunc(LerpTimeLineFinishCallBack);
		LerpTimeline.SetTimelineLength(0.25);

		FOnTimelineFloat Curve2Callback;
		Curve2Callback.BindUFunction(this, FName("AIUpdateDoor"));
		AIOpenLerpTimeline.AddInterpFloat(Curve1, Curve2Callback);
		//AIOpenLerpTimeline.SetTimelineFinishedFunc(LerpTimeLineFinishCallBack);
		AIOpenLerpTimeline.SetTimelineLength(0.25);
	}
}

void AHideActor::CloseLockerSetting()
{
	if (Curve1 != nullptr) {
		FOnTimelineFloat Curve1Callback;
		//FOnTimelineEvent LerpTimeLineFinishCallBack;
		Curve1Callback.BindUFunction(this, FName("UpdateDoor"));
		//LerpTimeLineFinishCallBack.BindUFunction(this, FName("DelayMove"));
		CloseLerpTimeline.AddInterpFloat(Curve1, Curve1Callback);
		//LerpTimeline.SetTimelineFinishedFunc(LerpTimeLineFinishCallBack);
		CloseLerpTimeline.SetTimelineLength(0.25);
	}
}

void AHideActor::UpdateDoor(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("%f"),Value);
	float DoorRotatefloat = UKismetMathLibrary::Lerp(0, 90, Value);
	LockerDoor->SetRelativeRotation(FRotator(0, DoorRotatefloat,0));
}

void AHideActor::AIUpdateDoor(float Value)
{
	float DoorRotatefloat = UKismetMathLibrary::Lerp(0, 90, Value);
	LockerDoor->SetRelativeRotation(FRotator(0, DoorRotatefloat, 0));
}

void AHideActor::FinishingUpdateDoor()
{	
	HGCharacter->SetActorEnableCollision(false);
	HGCharacter->GetCharacterMovement()->DisableMovement();
	GetWorld()->GetTimerManager().SetTimer(CharacterMoveTimer, this, &AHideActor::MoveCharacter, GetWorld()->GetDeltaSeconds(), true);
}

void AHideActor::DelayMove()
{
	FTimerHandle MoveDelay;
	GetWorld()->GetTimerManager().SetTimer(MoveDelay, this, &AHideActor::FinishingUpdateDoor, 0.3f, false);
}

void AHideActor::DelayDoorClose()
{
	FTimerHandle MoveDelay;
	GetWorld()->GetTimerManager().SetTimer(MoveDelay, this, &AHideActor::CloseLockerDoor, 0.3f, false);
	//GetWorld()->GetTimerManager().SetTimer(MoveDelay, this, &AHideActor::FinishingUpdateDoor, 0.2f, false);
}

