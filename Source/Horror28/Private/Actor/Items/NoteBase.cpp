// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Items/NoteBase.h"
#include "Components/TextRenderComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "HUD/Interface/PickupWidget.h"
#include "Character/HGCharacter.h"
#include <EnhancedInputSubsystems.h>
#include <EnhancedInputComponent.h>
#include "Camera\CameraComponent.h"
#include "HUD/Inventory/NoteExaminationWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Actor/ExaminationNote.h"

// Sets default values
ANoteBase::ANoteBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PaperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PaperMeshComponent"));
	PaperMesh->SetupAttachment(GetRootComponent());
	SetRootComponent(PaperMesh);
	PaperMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	OverlapSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphereComponent"));
	OverlapSphereComponent->SetupAttachment(PaperMesh);
	OverlapSphereComponent->SetSphereRadius(500.f);
	

	PickupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));
	PickupWidgetComponent->SetupAttachment(PaperMesh);
	PickupWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	PickupWidgetComponent->SetDrawSize(FVector2D(300.f, 300.f));
	PickupWidgetComponent->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
	PickupWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	PickupWidgetComponent->SetVisibility(false);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TEXT_Render"));
	TextRender->SetupAttachment(PaperMesh);
	TextRender->VerticalAlignment = EVerticalTextAligment::EVRTA_TextTop;
	TextRender->SetWorldSize(3);
	TextRender->SetRelativeRotation(FRotator(90,90,0));
	TextRender->SetTextRenderColor(FColor::Black);
	TextRender->SetRelativeLocation(FVector(-10, -15, 0.001));
	NoteText = FText::FromString(TEXT("현재 위치는 괴물이 출몰하여<br>위험합니다. 도주하십시오!"));
	

	BackTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Back_TEXT_Render"));
	BackTextRender->SetupAttachment(PaperMesh);
	BackTextRender->VerticalAlignment = EVerticalTextAligment::EVRTA_TextTop;
	BackTextRender->SetWorldSize(3);
	BackTextRender->SetRelativeRotation(FRotator(-90, -90, 0));
	BackTextRender->SetTextRenderColor(FColor::Black);
	BackTextRender->SetRelativeLocation(FVector(10, -15, -0.12));
	BackNoteText = FText::FromString(TEXT("어떤 종이는 뒷부분에 <br>중요한 단서가 있습니다."));
	

	//OverlapSphereComponent->bHiddenInGame = false;
	PickupDistance = 150;
	bIsCanPickup = false;
	bIsUpdateActor = false;
	OverlapSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ANoteBase::SphereBeginOverlap);
	OverlapSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ANoteBase::SphereEndOverlap);
}

// Called when the game starts or when spawned
void ANoteBase::BeginPlay()
{
	Super::BeginPlay();
	
	BackTextRender->SetText(BackNoteText);
	/*ReSetText*/
	TextRender->SetText(NoteText);
	TextRender->SetText(NoteText);
	BackTextRender->SetText(BackNoteText);
	ItemPickupWidget = Cast<UPickupWidget>(PickupWidgetComponent->GetUserWidgetObject());
	TArray<AActor*> ExaminationNoteActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AExaminationNote::StaticClass(), ExaminationNoteActors);
	ExaminationNoteRef = Cast<AExaminationNote>(ExaminationNoteActors[0]);
}

// Called every frame
void ANoteBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsUpdateActor) {
		//PickupWidgetComponent->GetLocati
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.bTraceComplex = false;
		if (HGCharacter) {
			bool bIsA = GetWorld()->LineTraceSingleByChannel(HitResult, PaperMesh->GetComponentLocation(), HGCharacter->ViewCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility, CollisionParams);
			if (!bIsA) {
				PickupWidgetComponent->SetVisibility(true);
				//GEngine->AddOnScreenDebugMessage(3, 1, FColor::Blue, FString(TEXT("Not Hide")), false);
				PickupWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(PickupWidgetComponent->GetComponentLocation(), HGCharacter->ViewCamera->GetComponentLocation()));
				float InteractDistance = UKismetMathLibrary::Vector_Distance(HitResult.TraceStart, HitResult.TraceEnd);
				if (InteractDistance <= PickupDistance) {
					ItemPickupWidget->TogglePrompt(true);
					bIsCanPickup = true;
					AddPickupContext();
				}
				else {
					ItemPickupWidget->TogglePrompt(false);
					bIsCanPickup = false;
				}
			}
			else {
				//GEngine->AddOnScreenDebugMessage(3, 1, FColor::Blue, FString(TEXT("Hide")), false);
				ItemPickupWidget->TogglePrompt(false);
				PickupWidgetComponent->SetVisibility(false);

			}
		}

	}
}

void ANoteBase::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass())) {
		HGCharacter = Cast<AHGCharacter>(OtherActor);
		if (HGCharacter) {

			AddPickupContext();
			//#include "Kismet/GameplayStatics.h"

			//DisableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			//GetWorld()->GetFirstPlayerController()->DisableInput(nullptr);
			//this->DisableInput(GetWorld()->GetFirstPlayerController());
			PickupWidgetComponent->SetVisibility(true);
			bIsUpdateActor = true;

		}
	}
}

void ANoteBase::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass())) {
		HGCharacter = Cast<AHGCharacter>(OtherActor);
		if (HGCharacter) {
			FreePickupContext();
			ItemPickupWidget->TogglePrompt(false);
			PickupWidgetComponent->SetVisibility(false);
			bIsUpdateActor = false;
		}
	}
}

void ANoteBase::AddPickupContext()
{
	//if (!PickupContext || !InputAction) return;
	//UE_LOG(LogTemp, Display, TEXT("overlap"));

	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
	//{

		// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
		//if (!Subsystem->HasMappingContext(PickupContext)) {
		//	Subsystem->AddMappingContext(PickupContext, 2);
		//}
	//}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		//EnhancedInputComponent->ClearBindingsForObject(this);
		EnhancedInputComponent->ClearBindingsForObject(this);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &ANoteBase::ViewExamination);
	}

}

void ANoteBase::FreePickupContext()
{
	//if (!PickupContext || !InputAction) return;

	//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
	//{
		//if (Subsystem->HasMappingContext(PickupContext)) {
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
		{
			EnhancedInputComponent->ClearBindingsForObject(this);
		}
		//Subsystem->RemoveMappingContext(PickupContext);

		//}
	//}

}

void ANoteBase::ViewExamination()
{
	if (bIsCanPickup) {
		if (UWorld* World = GetWorld()) {
			if (APlayerController* controller = World->GetFirstPlayerController()) {
				if (IsValid(NoteExaminationWidget)) {
					NoteExaminationWidget->UpdateNoteExamin(this);
					NoteExaminationWidget->AddToViewport(5);
				}
				else {
					NoteExaminationWidget = CreateWidget<UNoteExaminationWidget>(controller, NoteExaminationWidgetClass);
					NoteExaminationWidget->ExaminationNoteRef = ExaminationNoteRef;
					NoteExaminationWidget->UpdateNoteExamin(this);
					NoteExaminationWidget->ResetRot();
					NoteExaminationWidget->AddToViewport(5);
				}
				
				//NoteExaminationWidget->RemoveFromParent();
				//NoteExaminationWidget->BeginDestroy()
			}
		}
		
	}
}
