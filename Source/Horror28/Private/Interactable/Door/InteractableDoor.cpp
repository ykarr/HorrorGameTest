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
	// Timeline�� �����մϴ�.
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
	//��� Ŀ�갡 ������ �� Ÿ�Ӷ����� �����մϴ�.
	if (Curve1 != nullptr) {
		// ������ Curve�� ����� Callback �Լ����Դϴ�.
		FOnTimelineFloat Curve1Callback;
		FOnTimelineEvent FinishDoorTimeline;
		FinishDoorTimeline.BindUFunction(this,FName("DoorOpenFinish"));
		//FOnTimelineVector Curve2Callback;
		//FOnTimelineLinearColor Curve3Callback;

		// Timeline�� ������ �� ������ Callback �Լ����Դϴ�.
		//FOnTimelineEvent LerpTimelineFinishedCallback;

		// Callback �Լ��� ����� �Լ��� ���ε��մϴ�.
		// ���ε� �ϴ� �Լ����� UFUNCTION ��ũ�ΰ� ����Ǿ���մϴ�.
		
		
		Curve1Callback.BindUFunction(this, FName("DoorOpen"));
		//Curve2Callback.BindUFunction(this, FName("�Լ��̸�2"));
		//Curve3Callback.BindUFunction(this, FName("�Լ��̸�3"));
		//LerpTimelineFinishedCallback.BindUFunction(this, FName("�Լ��̸�4"));

		// Timeline�� Curve�� Curve�� ����� Callback �Լ��� �߰��մϴ�.
		LerpTimeline.SetTimelineFinishedFunc(FinishDoorTimeline);
		LerpTimeline.AddInterpFloat(Curve1, Curve1Callback);
		//LerpTimeline.AddInterpFloat(Curve2, Curve2Callback);
		//LerpTimeline.AddInterpFloat(Curve3, Curve3Callback);

		// Timeline�� ������ ȣ���� Cabllback �Լ��� �߰��մϴ�.
		//LerpTimeline.SetTimelineFinishedFunc(LerpTimelineFinishedCallback);

		// Timeline�� ���̸� �����մϴ�.
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
