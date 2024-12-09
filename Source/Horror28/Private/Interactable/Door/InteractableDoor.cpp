// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/Door/InteractableDoor.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyBase.h"
AInteractableDoor::AInteractableDoor():bIsDoorClose(true)
{
	PrimaryActorTick.bCanEverTick = true;
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	DoorMesh->SetupAttachment(RootComponent);
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("EnemyCheckBox"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AInteractableDoor::CheckEnemyBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AInteractableDoor::CheckEnemyEndOverlap);
	bIsCanInteract = true;
}

void AInteractableDoor::BeginPlay()
{
	Super::BeginPlay();
	DoorOpenSetting();
}

void AInteractableDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LerpTimeline.TickTimeline(DeltaTime);
}

void AInteractableDoor::DoorOpen(float Value)
{
	float DoorRotatefloat= FMath::Lerp(0, -90, Value);
	DoorMesh->SetRelativeRotation(FRotator(0, DoorRotatefloat,0 ));
}

void AInteractableDoor::DoorOpenFinish()
{
	DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

}

void AInteractableDoor::Interact()
{
	// Timeline을 실행합니다.
	if (bIsCanInteract) {
		if (bIsDoorClose) {
			//LerpTimeline.Play();
			DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			LerpTimeline.PlayFromStart();
			bIsDoorClose = false;
		}
		else {
			DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
			//LerpTimeline.Reverse();
			LerpTimeline.ReverseFromEnd();
			
			bIsDoorClose = true;
		}
	}
	
	
}
//Door TimelineSetting
void AInteractableDoor::DoorOpenSetting()
{
	//모든 커브가 존재할 때 타임라인을 설정합니다.
	if (Curve1 != nullptr) {
		// 지정한 Curve에 사용할 Callback 함수들입니다.
		FOnTimelineFloat Curve1Callback;
		FOnTimelineEvent FinishDoorTimeline;
		FinishDoorTimeline.BindUFunction(this,FName("DoorOpenFinish"));
		//FOnTimelineVector Curve2Callback;
		//FOnTimelineLinearColor Curve3Callback;

		// Timeline이 끝났을 때 실행할 Callback 함수들입니다.
		//FOnTimelineEvent LerpTimelineFinishedCallback;

		// Callback 함수에 사용할 함수를 바인드합니다.
		// 바인드 하는 함수에는 UFUNCTION 매크로가 적용되어야합니다.
		
		
		Curve1Callback.BindUFunction(this, FName("DoorOpen"));
		//Curve2Callback.BindUFunction(this, FName("함수이름2"));
		//Curve3Callback.BindUFunction(this, FName("함수이름3"));
		//LerpTimelineFinishedCallback.BindUFunction(this, FName("함수이름4"));

		// Timeline에 Curve와 Curve를 사용할 Callback 함수를 추가합니다.
		LerpTimeline.SetTimelineFinishedFunc(FinishDoorTimeline);
		LerpTimeline.AddInterpFloat(Curve1, Curve1Callback);
		//LerpTimeline.AddInterpFloat(Curve2, Curve2Callback);
		//LerpTimeline.AddInterpFloat(Curve3, Curve3Callback);

		// Timeline을 끝낼때 호출할 Cabllback 함수를 추가합니다.
		//LerpTimeline.SetTimelineFinishedFunc(LerpTimelineFinishedCallback);

		// Timeline의 길이를 설정합니다.
		LerpTimeline.SetTimelineLength(LerpTimelineLength);
	}
}

void AInteractableDoor::CheckEnemyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AEnemyBase::StaticClass())) {
		DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		//LerpTimeline.Play();
		LerpTimeline.PlayFromStart();
		bIsDoorClose = false;
		bIsCanInteract = false;
	}
}

void AInteractableDoor::CheckEnemyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AEnemyBase::StaticClass())) {
		DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		//LerpTimeline.Reverse();
		LerpTimeline.ReverseFromEnd();
		bIsDoorClose = true;
		bIsCanInteract = true;
	}
}
