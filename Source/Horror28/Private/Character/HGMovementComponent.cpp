// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HGMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/HGCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/PlayerController.h"

#include "Character/CameraShake/HGRunCameraShake.h"
#include "Character/CameraShake/HGWalkCameraShake.h"

#include "HUD/Interface/AttributeProgressWidget.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
UHGMovementComponent::UHGMovementComponent() :CrouchHalfHeight(44.f) ,MaxWalkSpeed(250.f), SprintSpeed(500.f), JumpZ(400.f),
MaxStamina(100.f), MinStamina(0), CurrentStamina(100.f),
CrouchSpeed(150.f) 
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UHGMovementComponent::FloatTemp() {

}
void UHGMovementComponent::InitalCharacterState()
{
	PlayerMovement = HGCharacter->GetCharacterMovement();
	PlayerMovement->MaxWalkSpeed = MaxWalkSpeed; //공포게임이라 살짝 느리게.
	PlayerMovement->JumpZVelocity = JumpZ;
}
// Called when the game starts
void UHGMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	if (Curve1 != nullptr) {
		FOnTimelineFloat Curve1Callback;
		Curve1Callback.BindUFunction(this, FName("ShortenPlayerCapsule"));
		LerpTimeline.AddInterpFloat(Curve1, Curve1Callback);
		LerpTimeline.SetTimelineLength(0.5);
	}
	if (LeanCurve!=nullptr) {
		FOnTimelineFloat LeanTimelineFloat;
		LeanTimelineFloat.BindUFunction(this, FName("RightUpdateLean"));
		RightLeanTimeline.AddInterpFloat(LeanCurve, LeanTimelineFloat);
		RightLeanTimeline.SetTimelineLength(0.3);
		
	}
	if (LeanCurve != nullptr) {
		FOnTimelineFloat LeanTimelineFloat;
		LeanTimelineFloat.BindUFunction(this, FName("LeftUpdateLean"));
		LeftLeanTimeline.AddInterpFloat(LeanCurve, LeanTimelineFloat);
		LeftLeanTimeline.SetTimelineLength(0.3);
	}
	FOnTimelineEvent FootStepEvent;
	FOnTimelineFloat FloatTimeline;
	FloatTimeline.BindUFunction(this,FName("FloatTemp"));
	FootStepTimeline.AddInterpFloat(Curve1,FloatTimeline);
	FootStepEvent.BindUFunction(this, FName("PlayFootStepSound"));
	FootStepTimeline.AddEvent(0.5, FootStepEvent);
	FootStepTimeline.AddEvent(1, FootStepEvent);
	FootStepTimeline.SetLooping(true);
	FootStepTimeline.SetTimelineLength(1);
	FootStepTimeline.PlayFromStart();
	/*그냥 Static Class 때려박아도 되지만 그냥 안전하게 오류검사하기 위해.*/
	HGWalkCameraShakeClass = UHGWalkCameraShake::StaticClass();
	HGRunCameraShakeClass = UHGRunCameraShake::StaticClass();
	//
	//AttributeProgressWidget->SetStaminaBarPercent(1);

}


void UHGMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	LerpTimeline.TickTimeline(DeltaTime);
	FootStepTimeline.TickTimeline(DeltaTime);
	RightLeanTimeline.TickTimeline(DeltaTime);
	LeftLeanTimeline.TickTimeline(DeltaTime);
	if (PlayerMovement) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovement->Velocity);
		FootStepTimeline.SetPlayRate(UKismetMathLibrary::MapRangeUnclamped(GroundSpeed,CrouchSpeed, MaxWalkSpeed,0.7,1));
		//UE_LOG(LogTemp, Display, TEXT("%f"), FootStepTimeline.GetPlayRate());
		//GEngine->AddOnScreenDebugMessage(2, 0.1, FColor::Red, FString::Printf(TEXT("Current Speed: %f"), GroundSpeed), 1);
		if (GroundSpeed >= MaxWalkSpeed && GetWorld()->GetFirstPlayerController()) {
			if (!PlayerMovement->IsFalling()) {
				float PlayCameraShakeScale = UKismetMathLibrary::MapRangeClamped(GroundSpeed, 0, MaxWalkSpeed, 0.f, 1.f);
				if (GroundSpeed >= SprintSpeed && HGRunCameraShakeClass) {//Sprint Camera
					//GEngine->AddOnScreenDebugMessage(3, 0.1, FColor::Red, FString("Sprint"), 1);
					GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HGRunCameraShakeClass, PlayCameraShakeScale);
					//GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HGRunCameraShakeClass, PlayCameraShakeScale);
				}
				else if (HGWalkCameraShakeClass) {//Walk Camera
					//GEngine->AddOnScreenDebugMessage(3, 0.1, FColor::Red, FString("Walk"), 1);
					GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HGWalkCameraShakeClass, PlayCameraShakeScale);
					//GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(HGWalkCameraShakeClass, PlayCameraShakeScale);
				}

			}
		}

	}
}
void UHGMovementComponent::HGCharacterSprint()
{
	if (bIsCrouching) return;
	GetWorld()->GetTimerManager().ClearTimer(RegenStaminaTimerHandle);
	//GetWorld()->GetTimerManager().ClearTimer(RegenStamina);
	GroundSpeed = UKismetMathLibrary::VSizeXY(PlayerMovement->Velocity);
	if (CurrentStamina > MinStamina&& GroundSpeed > 5) {
		PlayerMovement->MaxWalkSpeed = SprintSpeed;
		GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, this, &UHGMovementComponent::SprintTimer, 0.1, true);
	}
}

void UHGMovementComponent::HGCharacterCrouching()
{
	
	if (bIsCrouching) {
		GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
		PlayerMovement->MaxWalkSpeed = CrouchSpeed;
		LerpTimeline.Play();
	}
	else {
		LerpTimeline.Reverse();
		PlayerMovement->MaxWalkSpeed = MaxWalkSpeed;
	}
}

float UHGMovementComponent::GetStaminaBarPercent()
{
	
	return (CurrentStamina - MinStamina) / MaxStamina;
}

void UHGMovementComponent::SetStaminaWidget()
{
	//AttributeProgressWidget->SetStaminaBarPercent(GetStaminaBarPercent());
}

void UHGMovementComponent::StopSprint()
{
	if (bIsCrouching) return;
	PlayerMovement->MaxWalkSpeed = MaxWalkSpeed;
	//GetWorld()->GetTimerManager().SetTimer(SprintTimerHandle, this, &UHGMovementComponent::SprintTimer, 0.1, true);
	GetWorld()->GetTimerManager().ClearTimer(SprintTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(RegenCooltime);
	//람다는 익명함수 표현식. [capture list] (parameters) -> return-type { body }
	GetWorld()->GetTimerManager().SetTimer(RegenCooltime, FTimerDelegate::CreateLambda([&]()
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(SprintTimerHandle)) {
			GetWorld()->GetTimerManager().ClearTimer(RegenStaminaTimerHandle);
			GetWorld()->GetTimerManager().SetTimer(RegenStaminaTimerHandle, this,&UHGMovementComponent::RegenStamina , 0.1 , true);
		}
	}), 3.f, false);
}

void UHGMovementComponent::RegenStamina()
{
	CurrentStamina = FMath::Clamp(CurrentStamina + 1, MinStamina, MaxStamina);
	//GEngine->AddOnScreenDebugMessage(1, 0.1, FColor::Red, FString::Printf(TEXT("Current Stamina: %f"), CurrentStamina), 1);
	//AttributeProgressWidget->SetStaminaBarPercent(GetStaminaBarPercent());
	SetStaminaWidget();
	if (CurrentStamina >= MaxStamina) {
		GetWorld()->GetTimerManager().ClearTimer(RegenStaminaTimerHandle);
		UE_LOG(LogTemp, Display, TEXT("End"));
		return;
	}
}
void UHGMovementComponent::SprintTimer()
{
	//When Player Stop -> Sprint stop.
	if (GroundSpeed <= 1) {StopSprint();return;}
	CurrentStamina=FMath::Clamp(CurrentStamina-1, MinStamina, MaxStamina);
	//AttributeProgressWidget->SetStaminaBarPercent(GetStaminaBarPercent());
	SetStaminaWidget();
	//GEngine->AddOnScreenDebugMessage(1,0.1,FColor::Red, FString::Printf(TEXT("Current Stamina: %f"), CurrentStamina),1);//Debugging message
	if (CurrentStamina <= MinStamina) { //Not Enough stamina
		StopSprint();
	}
}
//When the player crouches, set the player height
void UHGMovementComponent::ShortenPlayerCapsule(float Value)
{
	HGCharacter->GetCapsuleComponent()->SetCapsuleHalfHeight(FMath::Lerp(88, CrouchHalfHeight, Value));
}

void UHGMovementComponent::PlayFootStepSound()
{
	//UE_LOG(LogTemp, Display, TEXT("play"));
	if (GroundSpeed > 10.f&&!PlayerMovement->IsFalling()) {
		FHitResult HitResult;
		FVector StartLoc(HGCharacter->GetActorLocation());
		FVector EndLoc = StartLoc + (HGCharacter->GetActorUpVector() * -200);
		//FVector a = FVector(0, 0, 0);
		TArray<AActor*> ActorToIgnore;
		ActorToIgnore.Add(HGCharacter);
		//GetWorld()->LineTrace
		//FCollisionResponseParams CollisionResponseParms;
		//if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, ETraceTypeQuery::TraceTypeQuery1, false, ActorToIgnore,EDrawDebugTrace::None,HitResult,true)) {
		//if (UKismetSystemLibrary::LineTraceSingle(GetWorld(), StartLoc, EndLoc, ETraceTypeQuery::TraceTypeQuery1, false, ActorToIgnore, EDrawDebugTrace::ForDuration, HitResult, true, FColor::Red, FColor::Green, 2)) {
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(GetOwner());
		TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = true;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, TraceParams)) {
			//if (HitResult.PhysMaterial.IsValid()) { // Check if Hit.PhysMaterial is valid
               USoundBase* FootStepSound;
			   
               switch (HitResult.PhysMaterial->SurfaceType) {
					case SurfaceType1: FootStepSound = FootStepSoundArray[0];  break; // Wood
                    case SurfaceType2: FootStepSound = FootStepSoundArray[1];  break; // Concrete
                    default: FootStepSound = FootStepSoundArray[0]; break;
                }
				
                UGameplayStatics::PlaySoundAtLocation(this, FootStepSound, HitResult.ImpactPoint, UKismetMathLibrary::MapRangeUnclamped(GroundSpeed, CrouchSpeed, MaxWalkSpeed, 0.3, 1));
           // }
		}
	}
}

void UHGMovementComponent::OnLanding()
{
	FHitResult HitResult;
	FVector StartLoc(HGCharacter->GetActorLocation());
	FVector EndLoc = StartLoc + (HGCharacter->GetActorUpVector() * -200);
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(HGCharacter);
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(GetOwner());
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLoc, EndLoc, ECollisionChannel::ECC_Visibility, TraceParams)) {
		
			USoundBase* FootStepSound;
			switch (HitResult.PhysMaterial->SurfaceType) {
			case SurfaceType1: FootStepSound = FootStepSoundArray[0];  break; // Wood
			case SurfaceType2: FootStepSound = FootStepSoundArray[1];  break; // Concrete
			default: FootStepSound = FootStepSoundArray[0]; break;
			}
			UGameplayStatics::PlaySoundAtLocation(this, FootStepSound, HitResult.ImpactPoint,2);
			FootStepTimeline.PlayFromStart();
	}
}

void UHGMovementComponent::LeanRight()
{
	RightLeanTimeline.PlayFromStart();
}

void UHGMovementComponent::UnLeanRight()
{
	RightLeanTimeline.ReverseFromEnd();
}

void UHGMovementComponent::LeanLeft()
{
	LeftLeanTimeline.PlayFromStart();
}

void UHGMovementComponent::UnLeanLeft()
{
	LeftLeanTimeline.ReverseFromEnd();
}

void UHGMovementComponent::RightUpdateLean(float Value)
{

	FRotator rot = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
	//FVector loc =
	FVector ViewLoc = HGCharacter->ViewCamera->GetRelativeLocation();
	HGCharacter->ViewCamera->SetRelativeLocation(FVector(ViewLoc.X, UKismetMathLibrary::Lerp(0, ViewLoc.Y + 5, Value), ViewLoc.Z));
	FRotator UpdateRot(rot.Pitch,rot.Yaw, UKismetMathLibrary::Lerp(0,20,Value));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(UpdateRot);
}
void UHGMovementComponent::LeftUpdateLean(float Value)
{
	FRotator rot = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetControlRotation();
	//FVector loc =
	FVector ViewLoc = HGCharacter->ViewCamera->GetRelativeLocation();
	HGCharacter->ViewCamera->SetRelativeLocation(FVector(ViewLoc.X, UKismetMathLibrary::Lerp(0 , ViewLoc.Y - 5, Value), ViewLoc.Z));
	FRotator UpdateRot(rot.Pitch, rot.Yaw, UKismetMathLibrary::Lerp(0, -20, Value));
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetControlRotation(UpdateRot);
}
