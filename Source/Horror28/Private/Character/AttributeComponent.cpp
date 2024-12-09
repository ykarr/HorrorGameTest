// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AttributeComponent.h"
#include "Kismet\KismetMathLibrary.h"
#include "HUD/Interface/AttributeProgressWidget.h"

#include "Kismet/KismetMaterialLibrary.h"
#include "Materials/MaterialParameterCollection.h"


//#include "Materials/MaterialInterface.h"
//#include "Materials/MaterialParameterCollectionInstance.h"
// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	MinHealth = 0.f;
	CurrentHealth = 100.f;
	MaxHealth = 100.f;
	//HealthParameterCollection = CreateDefaultSubobject< UMaterialParameterCollection>(TEXT("HealthParam"));
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	UpdateBloodScreen();

	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(97, 1, FColor::Green, FString::FormatAsNumber(CurrentHealth), true);
}
void UAttributeComponent::UpdateBloodScreen()
{
	/*70퍼 이하로 피가 내려가면 블러드 스크린작동.*/
	const float BloodScreen = UKismetMathLibrary::MapRangeClamped(CurrentHealth, MinHealth, (MaxHealth- MinHealth)*0.7, 0.5, 1);
	UKismetMaterialLibrary::SetScalarParameterValue(GetWorld(), HealthParameterCollection, FName("Amount"), BloodScreen);
		
}

void UAttributeComponent::SetHealthBarWidget()
{
	//AttributeProgressWidget->SetHealthBarPercent(GetHealthBarPercent());
}

void UAttributeComponent::RemoveHealth(float Amount)
{

	CurrentHealth = UKismetMathLibrary::Clamp(CurrentHealth-Amount,MinHealth,MaxHealth);
	UpdateBloodScreen();
}

void UAttributeComponent::AddHealth(float Amount)
{
	CurrentHealth = UKismetMathLibrary::Clamp(CurrentHealth + Amount, MinHealth, MaxHealth);
	UpdateBloodScreen();
}

bool UAttributeComponent::bIsHealthNotFull()
{
	return CurrentHealth < MaxHealth;
}

float UAttributeComponent::GetHealthBarPercent()
{
	return (CurrentHealth - MinHealth) / MaxHealth;
}



void UAttributeComponent::DebugDamage(float bIsDamaged)
{
	if (bIsDamaged) {
		RemoveHealth(10);
	}
	else {
		AddHealth(10);
	}

}