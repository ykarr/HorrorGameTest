// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable/InteractableActor.h"
#include "Components/TimeLineComponent.h"
#include "InteractableDoor.generated.h"

class UBoxComponent;
/**
 * 
 */
UCLASS()
class HORROR28_API AInteractableDoor : public AInteractableActor
{
	GENERATED_BODY()
public:
	AInteractableDoor();
	virtual void BeginPlay()override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, Category = "Interactable | Mesh")
	TObjectPtr<UStaticMeshComponent> DoorMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveFloat* Curve1;
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveVector* Curve2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UCurveLinearColor* Curve3;*/
	//#include "Components/TimeLineComponent.h"
	FTimeline LerpTimeline;
	// LerpTimeline의 길이를 설정하는 변수입니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	float LerpTimelineLength;

	virtual void Interact()override;
private:

	bool bIsDoorClose;
	void DoorOpenSetting();
	UPROPERTY(EditDefaultsOnly, Category = "EnemySetting")
	TObjectPtr<UBoxComponent> BoxComponent;
	UFUNCTION()
	void CheckEnemyBeginOverlap(  UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void CheckEnemyEndOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void DoorOpen(float Value);
	UFUNCTION()
	void DoorOpenFinish();
	bool bIsCanInteract;
};
