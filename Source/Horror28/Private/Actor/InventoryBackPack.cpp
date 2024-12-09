// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/InventoryBackPack.h"
#include "Components/SphereComponent.h"

#include "HUD/Interface/PickupWidget.h"
#include "Components/WidgetComponent.h"

#include "Character/InventoryComponent.h"
#include "Character/HGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "EnhancedInputComponent.h"
//#include "Character/HGCharacter.h"
// Sets default values
AInventoryBackPack::AInventoryBackPack():bIsUpdateActor(false), PickupDistance(150.f), bIsCanPickup(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BackPackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackPackMesh"));
	BackPackMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	BackPackMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BackPackMesh->SetupAttachment(GetRootComponent());
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	Sphere->SetupAttachment(BackPackMesh);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetSphereRadius(500.f);
	/*Debug*/
	Sphere->bHiddenInGame = false;
	
	//PickUpWidgetComponent = CreateDefaultSubobject<UPickupWidget>(TEXT("PickupWidget"));
	BaseWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidget"));
	BaseWidgetComponent->SetupAttachment(BackPackMesh);
	BaseWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	BaseWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	BaseWidgetComponent->SetDrawSize(FVector2D(300.f, 300.f));
	BaseWidgetComponent->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
	
	//BaseWidgetComponent->SetVisibility(false);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AInventoryBackPack::BackPackSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AInventoryBackPack::BackPackSphereEndOverlap);
}

// Called when the game starts or when spawned
void AInventoryBackPack::BeginPlay()
{
	Super::BeginPlay();
	
	PickupWidget = Cast<UPickupWidget>(BaseWidgetComponent->GetUserWidgetObject());

	
}

// Called every frame
void AInventoryBackPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsUpdateActor) {
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.bTraceComplex = false;
		IsValid(HGCharacter);
		bool bIsA = GetWorld()->LineTraceSingleByChannel(HitResult, BackPackMesh->GetComponentLocation(), HGCharacter->ViewCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility, CollisionParams);
		if (!bIsA) {
			BaseWidgetComponent->SetVisibility(true);
			BaseWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(BaseWidgetComponent->GetComponentLocation(), HGCharacter->ViewCamera->GetComponentLocation()));
			float InteractDistance = UKismetMathLibrary::Vector_Distance(HitResult.TraceStart, HitResult.TraceEnd);
			if (InteractDistance <= PickupDistance) {
				PickupWidget->TogglePrompt(true);
				bIsCanPickup = true;
				AddPickUpContext();

			}
			else {
				PickupWidget->TogglePrompt(false);
				bIsCanPickup = false;
			}
		}
		else {
			PickupWidget->TogglePrompt(false);
			BaseWidgetComponent->SetVisibility(false);
		}
	}
}
void AInventoryBackPack::BackPackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(5, 1, FColor::Yellow, FString("BeginOverlap"), true);
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass())) {
		HGCharacter = Cast<AHGCharacter>(OtherActor);
		if (HGCharacter) {
			AddPickUpContext();
			bIsUpdateActor = true;
			BaseWidgetComponent->SetVisibility(true);
		}
	}
}
void AInventoryBackPack::BackPackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//GEngine->AddOnScreenDebugMessage(5, 1, FColor::Yellow, FString("EndOverlap"), true);
	if (OtherActor && OtherActor->IsA(ACharacter::StaticClass())) {
		HGCharacter = Cast<AHGCharacter>(OtherActor);
		if (HGCharacter) {
			FreePickUpContext();
			bIsUpdateActor = false;
			PickupWidget->TogglePrompt(false);
			BaseWidgetComponent->SetVisibility(false);
		}
	}
}

void AInventoryBackPack::AddPickUpContext()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		//EnhancedInputComponent->ClearBindingsForObject(this);
		EnhancedInputComponent->ClearBindingsForObject(this);
		EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AInventoryBackPack::PickupItem);
	}
}

void AInventoryBackPack::FreePickUpContext()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
	}
}

void AInventoryBackPack::PickupItem()
{
	if (bIsCanPickup) {
		UE_LOG(LogTemp, Display, TEXT("Test"));
		HGCharacter->GetInventory()->AddMoreSlots(AmountOfSlots);
		Destroy();
		//GEngine->AddOnScreenDebugMessage(4, 1, FColor::Green, FString(TEXT("Hi")), false);
		//InventoryRef = HGCharacter->GetInventory();
		
		//if (InventoryRef->AddNewItem(CreateItemCopy(), ItemQuantity)) {
		//	UE_LOG(LogTemp, Display, TEXT("destroy"));
		
		//}
	}
}

