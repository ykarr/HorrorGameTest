// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/ItemBaseActor.h"
#include "Components/SphereComponent.h"
#include "Character/HGCharacter.h"
/*Inven*/
#include "HUD/Inventory/InventoryMenu.h"
#include "Character/InventoryComponent.h"
#include "HUD/Inventory/InventoryGrid.h"
/*Interact*/
#include "HUD/Interface/PickupWidget.h"
#include "Components/WidgetComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//#include "Components/InputComponent.h"


//#include "Kismet/GameplayStatics.h"

// Sets default values
AItemBaseActor::AItemBaseActor():bIsUpdateActor(false), PickupDistance(150.f), bIsCanPickup(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	//SceneRoot->SetupAttachment(GetRootComponent());
	//SetRootComponent(SceneRoot);
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(GetRootComponent());
	//ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SetRootComponent(ItemMesh);

	PickupWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PickupWidgetComponent"));
	PickupWidgetComponent->SetupAttachment(GetRootComponent());
	PickupWidgetComponent->SetRelativeLocation(FVector(0.f,0.f,50.f));
	//PickupWidgetComponent->SetRelativeRotation(FRotator(0,0,0));
	PickupWidgetComponent->SetDrawSize(FVector2D(300.f, 300.f));
	PickupWidgetComponent->SetRelativeScale3D(FVector(0.1,0.1,0.1));
	//PickupWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	PickupWidgetComponent->SetWidgetSpace(EWidgetSpace::World);

	PickupWidgetComponent->SetVisibility(false);

	/*Sphere Component*/
	ItemSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	ItemSphere->SetupAttachment(ItemMesh);
	ItemSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	//ItemSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	ItemSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ItemSphere->SetSphereRadius(480.f);
	

	/*debug*/
	ItemSphere->bHiddenInGame = true;

	//ItemMesh->SetSimulatePhysics(true);

	ItemSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemBaseActor::OnItemOverlap);
	ItemSphere->OnComponentEndOverlap.AddDynamic(this, &AItemBaseActor::OnItemEndOverlap);
	//PickupWidget->Setupattach
}

// Called when the game starts or when spawned
void AItemBaseActor::BeginPlay()
{
	Super::BeginPlay();
	
	InitalizedPickup();
	//DisableInput(GetWorld()->GetFirstPlayerController());

}

// Called every frame
void AItemBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//HGCharacter->ViewCamera->GetComponentLocation()
	
	if (bIsUpdateActor) {
		//PickupWidgetComponent->GetLocati
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.bTraceComplex = false;
		//CollisionParams.bReturnPhysicalMaterial = true;
		//GetWorld()->LineTrace
		if (HGCharacter) {


			bool bIsA = GetWorld()->LineTraceSingleByChannel(HitResult, ItemMesh->GetComponentLocation(), HGCharacter->ViewCamera->GetComponentLocation(), ECollisionChannel::ECC_Visibility, CollisionParams);
			if (!bIsA) {
				PickupWidgetComponent->SetVisibility(true);
				//GEngine->AddOnScreenDebugMessage(3, 1, FColor::Blue, FString(TEXT("Not Hide")), false);
				//PickupWidgetComponent->SetRelativeRotation();
				PickupWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(PickupWidgetComponent->GetComponentLocation(), HGCharacter->ViewCamera->GetComponentLocation()));
				float InteractDistance = UKismetMathLibrary::Vector_Distance(HitResult.TraceStart, HitResult.TraceEnd);

				//APlayerController* controller = GetWorld()->GetFirstPlayerController();
				if (InteractDistance <= PickupDistance) {
					ItemPickupWidget->TogglePrompt(true);
					bIsCanPickup = true;
					//GetWorld()->GetFirstPlayerController()->EnableInput(nullptr);
					//EnableInput(nullptr);
					AddPickupContext();
					//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
					//if (PlayerController)
					//{
					//	// 입력을 활성화합니다.
					//	PlayerController->DisableInput(nullptr);
					//}
					//EnableInput(GetWorld()->GetFirstPlayerController());

				}
				else {
					ItemPickupWidget->TogglePrompt(false);
					bIsCanPickup = false;
					FreePickupContext();
					//GetWorld()->GetFirstPlayerController()->DisableInput(nullptr);
					//DisableInput(nullptr);
					//this->DisableInput(GetWorld()->GetFirstPlayerController());
				}
			}
			else {
				//GEngine->AddOnScreenDebugMessage(3, 1, FColor::Blue, FString(TEXT("Hide")), false);
				ItemPickupWidget->TogglePrompt(false);
				PickupWidgetComponent->SetVisibility(false);
				//FreePickupContext();
				//FreePickupContext();
				//DisableInput(GetWorld()->GetFirstPlayerController());
			}
		}
	}
	
}

void AItemBaseActor::InitalizedPickup()
{
	if (ItemDataTable && !ItemID.IsNone()) {
		//TObjectPtr<FItemData> Item_Data = ItemDataTable->FindRow<FItemData>(ItemID, ItemID.ToString());
		ItemData = ItemDataTable->FindRow<FItemData>(ItemID, ItemID.ToString());
		bIsCanUseable = ItemData->bIsCanUseable;
		bItemStackable = ItemData->bIsStackable;
		if (ItemQuantity <= 0||!bItemStackable) ItemQuantity = 1;
		MaxStackData = ItemData->MaxStackAmount;
		ItemName = ItemData->ItemName;
		ItemBaseActorClass = ItemData->BaseActor;
		//UE_LOG(LogTemp, Display, TEXT("initpickup"));

		ItemPickupWidget = Cast<UPickupWidget>(PickupWidgetComponent->GetUserWidgetObject()); // 블루프린트에서 선택해야함.
		//if (ItemPickupWidget) {
		//	ItemPickupWidget->UpdateInteractWidget();
		//}
	}

}
AItemBaseActor* AItemBaseActor::CreateItemCopy() const
{
	//AItemBaseActor* ItemIn = NewObject< AItemBaseActor>(this->StaticClass());
	//AItemBaseActor* ItemIn = NewObject< AItemBaseActor>(this->StaticClass());
	//AItemBaseActor* ItemIn = NewObject<AItemBaseActor>(GetTransientPackage(), this->GetClass());
	AItemBaseActor* ItemIn = NewObject<AItemBaseActor>(GetTransientPackage(), ItemBaseActorClass);
	//AItemBaseActor* ItemIn = NewObject<AItemBaseActor>(HGCharacter, ItemBaseActorClass);//이건됨. 즉, 아우터인자가 필요한건데 이건 소유자를 생각하면되지않을까..? 일단은 다른걸로. Bandage에 주석처리.
	//AItemBaseActor* ItemIn = NewObject<AItemBaseActor>(this->GetClass());
	ItemIn->ItemDataTable = ItemDataTable;
	ItemIn->InventoryRef = InventoryRef;
	ItemIn->ItemBaseActorClass = ItemBaseActorClass;
	ItemIn->ItemID = ItemID;
	ItemIn->ItemData = ItemIn->ItemDataTable->FindRow<FItemData>(ItemIn->ItemID, ItemIn->ItemID.ToString());
	ItemIn->bItemStackable = ItemIn->ItemData->bIsStackable;
	if (ItemIn->ItemQuantity <= 0 || !ItemIn->bItemStackable) ItemIn->ItemQuantity = 1;
	ItemIn->MaxStackData = ItemIn->ItemData->MaxStackAmount;
	ItemIn->ItemName = ItemIn->ItemData->ItemName;
	ItemIn->bIsCanUseable = ItemIn->ItemData->bIsCanUseable;
	ItemIn->HGCharacter = HGCharacter;
	ItemIn->World = GetWorld();
	
	return ItemIn;
}

void AItemBaseActor::UseItem()
{

	//UE_LOG(LogTemp, Display, TEXT("baseActor"));
	ItemQuantity--;
	if (InventoryRef) {
		//UE_LOG(LogTemp, Display, TEXT("inven"));
		index = InventoryRef->GetSlotIndex(this);
		if (ItemQuantity < 1) {
			InventoryRef->UpdateItemSlot(index, nullptr);
			//APlayerController* player = Cast<APlayerController>(Controller);
			//AController* Controller = GetController();
			//UInventoryMenu* InvenMenu = Cast<UInventoryMenu>(GetWorld()->GetFirstPlayerController());
			//InvenMenu->HideDropDownBtn();
			InventoryRef->InventoryGridref->InventoryMenuRef->HideDropDownBtn();
		}
		else {
			InventoryRef->UpdateItemSlot(index, this);
		}
	}
}
void AItemBaseActor::OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, FString(TEXT("In Item")), false);

	if (OtherActor->IsA(AHGCharacter::StaticClass())) {
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

void AItemBaseActor::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AHGCharacter::StaticClass())) {
		HGCharacter = Cast<AHGCharacter>(OtherActor);
		if (HGCharacter) {
			FreePickupContext();
			ItemPickupWidget->TogglePrompt(false);
			PickupWidgetComponent->SetVisibility(false);
			bIsUpdateActor = false;
			
			/*InventoryRef = HGCharacter->GetInventory();
			if (InventoryRef->AddNewItem(CreateItemCopy(), ItemQuantity)) {
				UE_LOG(LogTemp, Display, TEXT("destroy"));
				Destroy();
			}*/
		}
	}
}

void AItemBaseActor::ItemPickUp()
{
	//if (bIsCanPickup) {
		//UE_LOG(LogTemp, Display, TEXT("Test"));
		//GEngine->AddOnScreenDebugMessage(4, 1, FColor::Green, FString(TEXT("Hi")), false);
		InventoryRef = HGCharacter->GetInventory();
		if (InventoryRef->AddNewItem(CreateItemCopy(), ItemQuantity)) {
			UE_LOG(LogTemp, Display, TEXT("destroy"));
			Destroy();
			
		}
	//}
		
}

void AItemBaseActor::AddPickupContext()
{
	if (!InputAction) return;
	//UE_LOG(LogTemp, Display, TEXT("overlap"));
	
		//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
		//{
		//	
		//	// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
		//	if (!Subsystem->HasMappingContext(PickupContext)) {
		//		Subsystem->AddMappingContext(PickupContext, 2);
		//	}
		//}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
		{
			//EnhancedInputComponent->ClearBindingsForObject(this);
			EnhancedInputComponent->ClearBindingsForObject(this);
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &AItemBaseActor::ItemPickUp);
		}
	
}

void AItemBaseActor::FreePickupContext()
{
	if (!InputAction) return;

		//if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetWorld()->GetFirstPlayerController()->GetLocalPlayer()))
		//{
		//	//if (Subsystem->HasMappingContext(PickupContext)) {
		//		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
		//		{
		//			EnhancedInputComponent->ClearBindingsForObject(this);
		//		}
		//		Subsystem->RemoveMappingContext(PickupContext);

		//	//}
		//}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(GetWorld()->GetFirstPlayerController()->InputComponent))
	{
		EnhancedInputComponent->ClearBindingsForObject(this);
	}
	
}