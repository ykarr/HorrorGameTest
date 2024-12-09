// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/FlashLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "HUD/Interface/AttributeProgressWidget.h"
// Sets default values for this component's properties
UFlashLightComponent::UFlashLightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MaxBatteryLevel=100.f;
	CurrentBatteryLevel=100.f;
	MinBatteryLevel=0.f;
	DepleteAmount=1.f;
	DepleteSpeed = 10.f;

}


// Called when the game starts
void UFlashLightComponent::BeginPlay()
{
	Super::BeginPlay();
	LightRef->SetVisibility(true);
	GetWorld()->GetTimerManager().ClearTimer(DepleteBatteryTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(DepleteBatteryTimerHandle, this, &UFlashLightComponent::DepleteBatteryLife, DepleteSpeed, true);
	SetIntensity();

}


// Called every frame
void UFlashLightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(98, 1, FColor::Green, FString::FormatAsNumber(CurrentBatteryLevel), true);
}

void UFlashLightComponent::SetBatteryTimer(bool bIsToogle)
{
	if (bIsToogle&& CurrentBatteryLevel>MinBatteryLevel) {
		LightRef->SetVisibility(true);
		GetWorld()->GetTimerManager().SetTimer(DepleteBatteryTimerHandle,this,&UFlashLightComponent::DepleteBatteryLife, DepleteSpeed,true);
	}
	else {
		LightRef->SetVisibility(false);
		GetWorld()->GetTimerManager().ClearTimer(DepleteBatteryTimerHandle);
	}
}

void UFlashLightComponent::DepleteBatteryLife()
{

	CurrentBatteryLevel=UKismetMathLibrary::Clamp(CurrentBatteryLevel- DepleteAmount, MinBatteryLevel, MaxBatteryLevel);
	SetIntensity();
	//SetBatteryWidget();
	if (CurrentBatteryLevel <= MinBatteryLevel) {
		LightRef->SetVisibility(false);
		GetWorld()->GetTimerManager().ClearTimer(DepleteBatteryTimerHandle);
	}
}

void UFlashLightComponent::AddBatteryLife(float Amount)
{
	CurrentBatteryLevel = UKismetMathLibrary::Clamp(CurrentBatteryLevel + Amount, MinBatteryLevel, MaxBatteryLevel);
}

void UFlashLightComponent::SetIntensity()
{
	
	if (LightRef->bUseIESBrightness) {
		
		Intensity=UKismetMathLibrary::MapRangeClamped(CurrentBatteryLevel,MinBatteryLevel,20,0,1);
		LightRef->SetIESBrightnessScale(Intensity);
	}
	else {
		//LightRef->SetIntensity();
	}
}

bool UFlashLightComponent::IsNotFullBattery()
{
	return CurrentBatteryLevel < MaxBatteryLevel;
}

float UFlashLightComponent::GetBatteryBarPercent()
{
	
	return (CurrentBatteryLevel - MinBatteryLevel) / MaxBatteryLevel;
}

void UFlashLightComponent::SetBatteryWidget()
{
	//AttributeProgressWidget->SetBatteryPercent(GetBatteryBarPercent());
}









