// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractInterface.h"
#include "Components/TimeLineComponent.h"
#include "HideActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnFinishLocker)

//class HGCharacter;
class IPlayerHideInterface;
class AHGCharacter;
class UArrowComponent;

UCLASS()
class HORROR28_API AHideActor : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	bool bIsCanInteract;
	//UStaticMesh* Locker;
	FOnFinishLocker OnFinishLocker;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Locker;
	UPROPERTY(EditAnywhere)
	//UStaticMesh* LockerDoor;
	UStaticMeshComponent* LockerDoor;
	UPROPERTY(EditAnywhere)
	UArrowComponent* PlayerPos;
	UPROPERTY(EditAnywhere)
	UArrowComponent* ExitPos;
	UPROPERTY(EditAnywhere)
	UArrowComponent* AILocation;
	FTimerHandle CharacterMoveTimer;
	AHGCharacter* HGCharacter;
protected:
	AHideActor();
	virtual void BeginPlay() override;
	void OpenLockerSetting();
	void CloseLockerSetting();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact() override;
	UFUNCTION()
	void UpdateDoor(float Value);
	UFUNCTION()
	void AIUpdateDoor(float Value);
	UFUNCTION()
	void FinishingUpdateDoor();
	UFUNCTION()
	void DelayMove();
	void DelayDoorClose();
	void MoveCharacter();
	void FinishMoveCharacter();
	void CloseLockerDoor();
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* Curve1;
	FTimeline LerpTimeline;
	FTimeline CloseLerpTimeline;

	FTimeline AIOpenLerpTimeline;
	//FTimeline AICloseLerpTimeline;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	float LerpTimelineLength;

	UPROPERTY(VisibleAnywhere, Category = "Interface")
	TScriptInterface<IPlayerHideInterface> PlayerHideInterface;
public:	
	void EnemyFound();

};
