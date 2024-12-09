// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/PlayerHideInterface.h"
#include "EnemyAIController.generated.h"

class UBehaviorTree;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UBlackboardData;
class UAnimMontage;
class UGameOverWidget;

/**
 * 
 */
UCLASS()
class HORROR28_API AEnemyAIController : public AAIController, public IPlayerHideInterface
{
	GENERATED_BODY()
public:
	AEnemyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	void RundAI();
	void StopAI();
	void SetAIPerceptionSystem();
	virtual void DidEnemySee(AHideActor* HideActor)override;
	virtual void LeftHidingSpot()override;
	UFUNCTION()
	void OnPlayerCharacterDetected(AActor* Actor, FAIStimulus Stimulus);
	void AttakPlayerMontage();
	UFUNCTION(BlueprintCallable)
	void PullOutOfHidding();
	UPROPERTY(EditAnywhere, Category = "AI")
	UBehaviorTree* EnemyBehaviorTree;
	UPROPERTY(EditAnywhere, Category = "AI")
	UBlackboardData* EnemyBlackBoardData;
	UPROPERTY(EditAnywhere, Category = "AI")
	UBlackboardComponent* EnemyBlackBoard;
	UPROPERTY(EditAnywhere)
	UAIPerceptionComponent* AIPerceptionComponent;
	UPROPERTY(EditAnywhere, Category = "AI")
	UAnimMontage* EnemyAttakMontage;
	UAISenseConfig_Sight* SightConfig;
	FTimerHandle DelayHandle;
	UPROPERTY(EditAnywhere, Category = "GameOver")
	TSubclassOf<UGameOverWidget> GameOverWidgetClass;

	AHideActor* HideActorRef;
};
