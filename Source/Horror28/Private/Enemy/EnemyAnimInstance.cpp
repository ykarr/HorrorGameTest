// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAnimInstance.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet\KismetMathLibrary.h"
#include "Character/HGCharacter.h"
//#include "GameFramework/Character.h"
void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (IsValid(TryGetPawnOwner())) {
		if (EnemyRef = Cast<AEnemyBase>(TryGetPawnOwner())) {
			if (EnemyRef) {
				EnemyMovement = EnemyRef->GetCharacterMovement();
			}
			
		}
	}
	
	/*쉬벌 이거 왜 안되는겨*/
	//ACharacter* tmp = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//HGCharacter = Cast<AHGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	//AHGCharacter* PlayerCh = Cast<AHGCharacter>(UGamePlayStatic);
	//
	//UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	
}

void UEnemyAnimInstance::NativeBeginPlay()
{
	
	//if (HGCharacter) {
	//	UE_LOG(LogTemp, Warning, TEXT("SiBal"));
	//	HGCharacter->TestText();
	//}
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//ACharacter* Character = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!HGCharacter) {
		HGCharacter = Cast<AHGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}
	if (EnemyMovement&& HGCharacter) {
		GroundSpeed = UKismetMathLibrary::VSizeXY(EnemyMovement->Velocity);
		Isfalling = EnemyMovement->IsFalling();
		FVector EnemyLoc = EnemyRef->GetActorLocation();
		FVector TargetLoc = EnemyRef->GetTargetPlayerLoc();
		FRotator HeadRot= UKismetMathLibrary::FindLookAtRotation(EnemyLoc, TargetLoc);
		if (HeadRot.Yaw > 45.f || HeadRot.Yaw < -45.f) {
			//Not See Player
			//HeadRotation =;
			HeadRotation = UKismetMathLibrary::RInterpTo(HeadRotation, FRotator(0, 0, 0), DeltaSeconds,0.5 );
		}
		else {
			//See Player
			FRotator TmpRot= FRotator(HeadRot.Roll, UKismetMathLibrary::ClampAngle(HeadRot.Yaw, -45.f, 45.f), HeadRot.Pitch * -1);
			HeadRotation = UKismetMathLibrary::RInterpTo(HeadRotation, TmpRot, DeltaSeconds, 0);
		}

		
	}
	
	
	
	
}
