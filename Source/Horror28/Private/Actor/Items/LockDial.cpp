// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Items/LockDial.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet\KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "HUD/Interface/ArrowWidget.h"
#include "Actor/Items/LockActor.h"

// Sets default values
ALockDial::ALockDial()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootScene->SetupAttachment(GetRootComponent());
	SetRootComponent(RootScene);
	LockDialMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LockDial"));
	LockDialMesh->SetupAttachment(GetRootComponent());
	//SetRootComponent(LockDialMesh);

	UpWidgetBaseComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UpWidgetBaseComponent"));
	UpWidgetBaseComponent->SetupAttachment(GetRootComponent());
	UpWidgetBaseComponent->SetWorldScale3D(FVector(0.001, 0.001, 0.001));
	UpWidgetBaseComponent->SetRelativeRotation(FRotator(180,-90,0));
	UpWidgetBaseComponent->SetRelativeLocation(FVector(0, 1, 1));

	DownWidgetBaseComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DownWidgetBaseComponent"));
	DownWidgetBaseComponent->SetupAttachment(GetRootComponent());
	DownWidgetBaseComponent->SetWorldScale3D(FVector(0.001, 0.001, 0.001));
	DownWidgetBaseComponent->SetRelativeRotation(FRotator(0, 90, 0));
	DownWidgetBaseComponent->SetRelativeLocation(FVector(0, 1, -1));

	DialNumber = 0;
	bIsCanRot = true;
	LerpTimelineLength=0.5;

}

// Called when the game starts or when spawned
void ALockDial::BeginPlay()
{
	Super::BeginPlay();
	/*UpBtnSetting*/
	UpArrowBtn= Cast<UArrowWidget>(UpWidgetBaseComponent->GetUserWidgetObject());
	UpArrowBtn->LockDialRef = this;
	UpArrowBtn->bIsAddRot = true;
	
	/*DownBtnSetting*/
	DownArrowBtn = Cast<UArrowWidget>(DownWidgetBaseComponent->GetUserWidgetObject());
	DownArrowBtn->LockDialRef = this;
	DownArrowBtn->bIsAddRot = false;
	
	InitTimeline();
	SetRandomNumber();
	//FindNumber();
	ArrowVisiblilty(false);
}

// Called every frame
void ALockDial::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LerpTimeline.TickTimeline(DeltaTime);
	LerpTimelineDown.TickTimeline(DeltaTime);
}

void ALockDial::SetRandomNumber()
{
	int32 RandNumberInt = UKismetMathLibrary::RandomIntegerInRange(0, 9);
	float TmpDialRotation = RandNumberInt * 36;
	DialRotation = TmpDialRotation;
	LockDialMesh->SetRelativeRotation(FRotator(0,0, TmpDialRotation));
	//UE_LOG(LogTemp, Display, TEXT("ffffffffffDialRot -> %f"), DialRotation);
	//FindNumber();
	DialNumber = DialRotation / 36;
}





void ALockDial::InitTimeline()
{
	if (Curve1) {
		
		FOnTimelineFloat TimelineFloat;
		FOnTimelineEvent LerpTimeLineFinishCallBack;
		TimelineFloat.BindUFunction(this, FName("UpdateLockDialRot"));
		LerpTimeLineFinishCallBack.BindUFunction(this, FName("FinishedUpdateDialRot"));
		//Tmp는 귀찮은 log가 계속 떠서 추가.
		LerpTimeline.AddInterpFloat(Curve1, TimelineFloat);
		//LerpTimeline.SetPropertySetObject(this); // Set which object the timeline should drive properties on
		//LerpTimeline.SetDirectionPropertyName(FName("TimelineDirection"));
		LerpTimeline.SetTimelineLength(LerpTimelineLength);
		LerpTimeline.SetTimelineFinishedFunc(LerpTimeLineFinishCallBack);

		FOnTimelineFloat TimelineFloatDown;
		FOnTimelineEvent LerpTimeLineFinishCallBackDown;
		TimelineFloatDown.BindUFunction(this, FName("UpdateLockDialRotDown"));
		LerpTimeLineFinishCallBackDown.BindUFunction(this, FName("FinishedUpdateDialRotDown"));
		//Tmp는 귀찮은 log가 계속 떠서 추가.
		LerpTimelineDown.AddInterpFloat(Curve1, TimelineFloatDown);
		//LerpTimelineDown.SetPropertySetObject(this); // Set which object the timeline should drive properties on
		//LerpTimelineDown.SetDirectionPropertyName(FName("TimelineDirection"));
		LerpTimelineDown.SetTimelineLength(LerpTimelineLength);
		LerpTimelineDown.SetTimelineFinishedFunc(LerpTimeLineFinishCallBackDown);
	}
	
}
void ALockDial::RotDial()
{
	if (bIsCanRot) {
		bIsCanRot = false;
		//LerpTimeline.PlayFromStart();
		//LerpTimeline.Play();
		LerpTimeline.PlayFromStart();
	}
}
void ALockDial::RotDialDown()
{
	if (bIsCanRot) {
		bIsCanRot = false;
		//LerpTimeline.ReverseFromEnd();
		LerpTimelineDown.PlayFromStart();
	}
}
void ALockDial::ArrowVisiblilty(bool bIsVisible)
{
	if (bIsVisible) {
		UpArrowBtn->SetVisibility(ESlateVisibility::Visible);
		DownArrowBtn->SetVisibility(ESlateVisibility::Visible);
	}
	else {
		UpArrowBtn->SetVisibility(ESlateVisibility::Collapsed);
		DownArrowBtn->SetVisibility(ESlateVisibility::Collapsed);
	}
	
}
void ALockDial::UpdateLockDialRot(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("Value : %f"),Value);
	float rot = DialRotation + UKismetMathLibrary::Lerp(0, 36, Value);
	/*if (TimelineDirection == ETimelineDirection::Forward) {
		rot = DialRotation + UKismetMathLibrary::Lerp(0, 36, Value);
	}
	else {
		rot = DialRotation  + UKismetMathLibrary::Lerp(-36, 0, Value);
	}*/
	LockDialMesh->SetRelativeRotation(FRotator(0,0, rot));
}
void ALockDial::UpdateLockDialRotDown(float Value)
{
	//UE_LOG(LogTemp, Display, TEXT("Value : %f"), Value);
	float rot = DialRotation + UKismetMathLibrary::Lerp(0, -36, Value);
	LockDialMesh->SetRelativeRotation(FRotator(0, 0, rot));
}
void ALockDial::FinishedUpdateDialRotDown()
{
	/*switch (TimelineDirection)
	{
	case ETimelineDirection::Forward:
		DialRotation += 36;
		break;
	case ETimelineDirection::Backward:
		DialRotation -= 36;
		break;
	default:
		break;
	}*/
	DialRotation -= 36;
	if (DialRotation >= 360) {
		DialRotation = 0;
		LockDialMesh->SetRelativeRotation(FRotator(0, 0, 0));
	}
	if (DialRotation < 0) {
		DialRotation = UKismetMathLibrary::MapRangeClamped(DialRotation, 0, -360, 360, 0);
		LockDialMesh->SetRelativeRotation(FRotator(0, 0, DialRotation));
	}
	FindNumber();
	bIsCanRot = true;
}
void ALockDial::FinishedUpdateDialRot()
{
	/*switch (TimelineDirection)
	{
	case ETimelineDirection::Forward:
		DialRotation += 36;
		break;
	case ETimelineDirection::Backward:
		DialRotation -= 36;
		break;
	default:
		break;
	}*/
	DialRotation += 36;
	if (DialRotation >= 360) {
		DialRotation = 0;
		LockDialMesh->SetRelativeRotation(FRotator(0, 0, 0));
	}
	if (DialRotation < 0) {
		DialRotation =UKismetMathLibrary::MapRangeClamped(DialRotation,0,-360,360,0 );
		LockDialMesh->SetRelativeRotation(FRotator(0, 0, DialRotation));
	}
	FindNumber();
	bIsCanRot = true;
}

void ALockDial::FindNumber()
{
	DialNumber = DialRotation / 36;
	UE_LOG(LogTemp, Display, TEXT("Random : %d"),DialNumber);
	bool bIsLockOpen=LockActorRef->bCheckCode();
	if (bIsLockOpen) {
		UE_LOG(LogTemp, Display, TEXT("Open"));
	}
}