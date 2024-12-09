// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/InteractableActor.h"

void AInteractableActor::Interact()
{
	UE_LOG(LogTemp, Display, TEXT("Interact"));
}

// Sets default values
AInteractableActor::AInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	InteractableMesh->SetupAttachment(RootComponent);
	SetRootComponent(InteractableMesh);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

