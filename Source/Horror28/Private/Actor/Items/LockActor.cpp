// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Items/LockActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Actor/Items/LockDial.h"
#include "Character/HGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Animation/AnimationAsset.h"
// Sets default values
ALockActor::ALockActor() :bIsLockOpen(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	//RootScene->SetupAttachment(GetRootComponent());
	//SetRootComponent(RootScene);

	LockMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LockMesh"));
	LockMesh->SetupAttachment(GetRootComponent());
	SetRootComponent(LockMesh);
	
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	ViewCamera->SetupAttachment(GetRootComponent());
	ViewCamera->SetRelativeLocation(FVector(0,15,-5));
	ViewCamera->SetRelativeRotation(FRotator(0,-90,0));
	ViewCamera->FieldOfView = 45;

	BoxExtent = FVector(48,32,22);
	BoxPos = FVector(0, -27, -15);
	LockBoxC = CreateDefaultSubobject<UBoxComponent>(TEXT("LockBox"));
	LockBoxC->SetBoxExtent(BoxExtent);
	LockBoxC->SetRelativeLocation(BoxPos);
	LockBoxC->SetupAttachment(GetRootComponent());
	LockBoxC->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LockBoxC->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	LockCombination[0] = 0;
	LockCombination[1] = 6;
	LockCombination[2] = 1;
	LockCombination[3] = 1;

	WidgetInteractionComponent = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteraction"));
	WidgetInteractionComponent->SetupAttachment(RootComponent);
	WidgetInteractionComponent->InteractionSource = EWidgetInteractionSource::Mouse;
}

// Called when the game starts or when spawned
void ALockActor::BeginPlay()
{
	Super::BeginPlay();
	HGCharacter = Cast<AHGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	PlayerMovement = HGCharacter->GetCharacterMovement();
	//PlayerMovement = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetCharacterMovement();


	//LockDialArray.SetNum(4);
	for (int i = 0; i < 4; i++) {

		//FActorSpawnParameters SpawnParams;
		//SpawnParams.Owner = this;
		//const FTransform SpawnTransform(GetActorRotation());
		ALockDial* Dial = GetWorld()->SpawnActor<ALockDial>(LockDialClass);
		LockDialArray.Add(Dial);
		FName SoketName;
		switch (i)
		{
			case 0: SoketName = "DialBone1Socket"; break;
			case 1: SoketName = "DialBone2Socket"; break;
			case 2: SoketName = "DialBone3Socket"; break;
			case 3: SoketName = "DialBone4Socket"; break;
			default: break;
		}
		//LockMesh->AttachToComponent( );
		//Dial->SetActorRotation(GetActorRotation());
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld,true);
		Dial->AttachToComponent(LockMesh, TransformRules, SoketName);
		Dial->LockActorRef = this;
	}
	
}

// Called every frame
void ALockActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALockActor::bCheckCode()
{
	if(bIsLockOpen) return false;
	for (int32 i = 0; i < 4; i++) {
		if (LockDialArray[i]->DialNumber != LockCombination[i]) {
			return false;
		}
	}
	bIsLockOpen = true;
	OpenAnim();
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, [this](){
		EscapeLockView();
		},2,false);
	return true;
}

void ALockActor::OpenAnim()
{
	LockMesh->PlayAnimation(OpenAnimation, false);
}

void ALockActor::Interact()
{
	//ViewCamera->GetOwner();
	float BlendTime=1.f;
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(this, BlendTime );
	PlayerMovement->DisableMovement();
	GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(true);
	HGCharacter->DisableInput(GetWorld()->GetFirstPlayerController());
	HGCharacter->bIsCanOpenInven = false;
	for (int32 i = 0; i < 4; i++) {
		LockDialArray[i]->ArrowVisiblilty(true);
	}
	/*input Setting*/
	GetWorld()->GetTimerManager().SetTimer(DelayHandle,this, &ALockActor::DelayStartFun,1, false);
	const FInputModeGameAndUI InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	
	
}

void ALockActor::DelayStartFun()
{
	UE_LOG(LogTemp, Display, TEXT("delay"));
	EnableActionInput();
}

void ALockActor::EnableActionInput()
{
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent)) {
		EnhancedInputComponent->ClearBindingsForObject(this);
		EnhancedInputComponent->BindAction(EscapeAction,ETriggerEvent::Triggered,this, &ALockActor::EscapeLockView);
		EnhancedInputComponent->BindAction(leftMouseBtn,ETriggerEvent::Started,this, &ALockActor::EnableLeftMouseBtn);
		EnhancedInputComponent->BindAction(leftMouseBtn,ETriggerEvent::Completed,this, &ALockActor::DisableLeftMouseBtn);
		//EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered,this, &ALockActor::EscapeLockView);
	}
}

void ALockActor::EscapeLockView()
{
	float BlendTime = 1.f;
	for (int32 i = 0; i < 4; i++) {
		LockDialArray[i]->ArrowVisiblilty(false);
	}
	const FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(GetWorld()->GetFirstPlayerController()->GetPawn(), BlendTime);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent)) {
		EnhancedInputComponent->ClearBindingsForObject(this);
	}
	HGCharacter->EnableInput(GetWorld()->GetFirstPlayerController());
	PlayerMovement->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorld()->GetFirstPlayerController()->ResetIgnoreLookInput();
	HGCharacter->bIsCanOpenInven = true;
	FTimerHandle DelayTimer;
	GetWorld()->GetTimerManager().SetTimer(DelayTimer, [this]() {
		if (bIsLockOpen) {
			for (int32 i = 0; i < 4; i++) {
				LockDialArray[i]->Destroy();
			}
			Destroy();
		}
	}, 2, false);
	
}

void ALockActor::EnableLeftMouseBtn()
{
	WidgetInteractionComponent->PressPointerKey(EKeys::LeftMouseButton);
}
void ALockActor::DisableLeftMouseBtn()
{
	WidgetInteractionComponent->ReleasePointerKey(EKeys::LeftMouseButton);
}
