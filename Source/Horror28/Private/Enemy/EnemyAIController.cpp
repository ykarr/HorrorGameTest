// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
/*behaviortree*/
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
/*perception*/
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
/*HUD*/
#include "HUD/GameOverWidget.h"
/*Actor*/
#include "Character/HGCharacter.h"
#include "Actor/HideActors/HideActor.h"
#include "Components/ArrowComponent.h"
/*animation*/
#include "Animation/AnimMontage.h"

AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	EnemyBlackBoard = CreateOptionalDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComponent"));
	
	//UAISenseConfig* Sence;
	SetAIPerceptionSystem();
	
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	EnemyBlackBoard->InitializeBlackboard(*EnemyBlackBoardData);
	
	RundAI();
}

void AEnemyAIController::RundAI()
{
	RunBehaviorTree(EnemyBehaviorTree);
	EnemyBlackBoard->SetValueAsBool(FName("bIsCanPlayJumpScareAudio"), true);
}

void AEnemyAIController::StopAI()
{
}

void AEnemyAIController::SetAIPerceptionSystem()
{
	SightConfig = CreateOptionalDefaultSubobject< UAISenseConfig_Sight>(TEXT("EnemySightConfig"));
	SightConfig->SightRadius=1000.f; //3000
	//SightConfig->SightRadius=250.f;
	SightConfig->LoseSightRadius=1250;//3000
	SightConfig->PeripheralVisionAngleDegrees = 75.f; //½Ã¾ß°¢.

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPlayerCharacterDetected);
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	
	//AIPerceptionComponent->On
}

void AEnemyAIController::DidEnemySee(AHideActor* HideActor)
{
	if (EnemyBlackBoard->GetValueAsBool(FName("bIsCanSeePlayer"))) {
		UE_LOG(LogTemp, Display, TEXT("Enemy"));
		//bIsCanKillHiddenPlayer TargetLocation
		EnemyBlackBoard->SetValueAsBool(FName("bIsCanKillHiddenPlayer"), true);
		//AIRotation
		EnemyBlackBoard->SetValueAsVector(FName("TargetLocation"), HideActor->AILocation->GetComponentLocation());
		EnemyBlackBoard->SetValueAsRotator(FName("AIRotation"), HideActor->AILocation->GetComponentRotation());
		HideActorRef=HideActor;
	}
}

void AEnemyAIController::LeftHidingSpot()
{
	EnemyBlackBoard->SetValueAsBool(FName("bIsCanKillHiddenPlayer"), false);
}

void AEnemyAIController::OnPlayerCharacterDetected(AActor* Actor, FAIStimulus Stimulus)
{
	//if (Actor&&Actor->IsA(AHGCharacter::StaticClass())) {
	if (Cast<AHGCharacter>(Actor)) {

		if (Stimulus.WasSuccessfullySensed()) {
			//GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, FString::Printf(TEXT("Find")));//Debugging message
			EnemyBlackBoard->SetValueAsBool(FName("bIsCanSeePlayer"), true);
			EnemyBlackBoard->SetValueAsBool(FName("bIsPursuingPlayer"), true);
			//GetWorld()->GetTimerManager().SetTimer(DelayHandle,[this](){
			//EnemyBlackBoard->SetValueAsBool(FName("bIsCanPlayJumpScareAudio"), false);
			//	}, 0.5f, false);
			
		}
		else{
			//GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, FString::Printf(TEXT("lose")));//Debugging message
			EnemyBlackBoard->SetValueAsBool(FName("bIsCanSeePlayer"), false);
			//EnemyBlackBoard->SetValueAsBool(FName("bIsCanPlayJumpScareAudio"), true);
		}
		//EnemyBlackBoard->SetValue
	}
}

void AEnemyAIController::AttakPlayerMontage()
{
	//UE_LOG(LogTemp, Display, TEXT("hi"));
	//ACharacter* EnemyActor= this->GetCharacter();
	UAnimInstance* AnimInstance = GetCharacter()->GetMesh()->GetAnimInstance();
	if (AnimInstance&& EnemyAttakMontage) {
		AnimInstance->Montage_Play(EnemyAttakMontage);
		AnimInstance->Montage_JumpToSection(FName("Attack2"), EnemyAttakMontage);
		//UGameOverWidget* GameOverWidget = Cast<UGameOverWidget>(CreateWidget(this, GameOverWidgetClass));
		UGameOverWidget* GameOverWidget = CreateWidget<UGameOverWidget>(GetWorld()->GetFirstPlayerController(), GameOverWidgetClass);
		GameOverWidget->AddToViewport(10);
		FInputModeUIOnly InputMode;
		GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
}

void AEnemyAIController::PullOutOfHidding()
{
	HideActorRef->EnemyFound();
}


