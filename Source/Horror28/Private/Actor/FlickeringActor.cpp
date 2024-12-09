// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FlickeringActor.h"
#include "Components/PointLightComponent.h"
#include "Engine/EngineTypes.h"
// Sets default values
AFlickeringActor::AFlickeringActor() :Intensity(5000), bIsMaxLight(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FlickeringLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("FlickeringLight"));
	FlickeringLight->SetupAttachment(RootComponent);
	DelayTime = 0.3;
}

// Called when the game starts or when spawned
void AFlickeringActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().ClearTimer(DelayTimerHandle);
	GetWorldTimerManager().SetTimer(DelayTimerHandle,this, &AFlickeringActor::LightFlickering , DelayTime, false);
}

// Called every frame
void AFlickeringActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFlickeringActor::LightFlickering()
{
	//UE_LOG(LogTemp, Display, TEXT("Light"));
	if (bIsMaxLight) {
		Intensity = 3500;
		bIsMaxLight = false;
	}
	else {
		Intensity = 5000;
		bIsMaxLight = true;
	}

	FlickeringLight->SetIntensity(Intensity);
	DelayTime = FMath::RandRange(0.1, 0.5);
	//UE_LOG(LogTemp, Display, TEXT("%f"),DelayTime);
	GetWorldTimerManager().SetTimer(DelayTimerHandle, this, &AFlickeringActor::LightFlickering, DelayTime, false);
}


