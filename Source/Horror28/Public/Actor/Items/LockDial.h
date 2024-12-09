// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimeLineComponent.h"
#include "LockDial.generated.h"

class UArrowWidget;
class UWidgetComponent;
class ALockActor;
//enum ETimelineDirection;
/*UENUM(BlueprintType)
enum class ETimelineDirection : uint8
{
	Forward UMETA(DisplayName = "Forward"),
	Backward UMETA(DisplayName = "Backward")
};*/

UCLASS()
class HORROR28_API ALockDial : public AActor
{
	GENERATED_BODY()
	
public:	
	TObjectPtr<ALockActor> LockActorRef;
	UPROPERTY(EditAnywhere, Category = "Widget")
	TObjectPtr<UWidgetComponent> UpWidgetBaseComponent;
	UPROPERTY(EditAnywhere, Category = "Widget")
	TObjectPtr<UWidgetComponent> DownWidgetBaseComponent;
	//UPROPERTY(EditAnywhere, Category = "Widget")
	//<UArrowWidget> ArrowBtnClass;
	TObjectPtr<UArrowWidget> UpArrowBtn;
	TObjectPtr<UArrowWidget> DownArrowBtn;
	UPROPERTY(EditAnywhere)
	USceneComponent* RootScene;
	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* LockDialMesh;
	int32 DialNumber;
	bool bIsCanRot;
	float DialRotation;
	UPROPERTY()
	float Tmp;
	
	void FindNumber();
	void RotDial();
	void RotDialDown();
	void ArrowVisiblilty(bool bIsVisible);
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", Meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
	UCurveFloat* Curve1;
	FTimeline LerpTimeline;
	FTimeline LerpTimelineDown;
	//UPROPERTY(EditDefaultsOnly, Category = "Timeline")
	//TEnumAsByte<ETimelineDirection::Type> TimelineDirection;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timeline", meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditDefaultsOnly, Category = "Timeline")
	float LerpTimelineLength;
protected:
	ALockDial();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SetRandomNumber();
	void InitTimeline();
	UFUNCTION()
	void UpdateLockDialRot(float Value);
	UFUNCTION()
	void UpdateLockDialRotDown(float Value);
	UFUNCTION()
	void FinishedUpdateDialRotDown();
	UFUNCTION()
	void FinishedUpdateDialRot();
	
};
