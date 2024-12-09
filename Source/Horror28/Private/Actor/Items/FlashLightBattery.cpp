// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Items/FlashLightBattery.h"
#include "Kismet/GameplayStatics.h"
#include "Character/FlashLightComponent.h"
#include "Character/HGCharacter.h"

#include "Level1/Ll1_GameStateBase.h"
//#include "Interface/ObjectiveCallInterface.h"
AFlashLightBattery::AFlashLightBattery()
{
	ItemID = "B_0001";
	BatteryAmount = 100.f;
}

void AFlashLightBattery::BeginPlay()
{
	Super::BeginPlay();

}

void AFlashLightBattery::UseItem()
{
	if (HGCharacter) {
		if (HGCharacter->FlashLightComponent->IsNotFullBattery()) {
			if (IsValid(World)) {
				Cast<ALl1_GameStateBase>(World->GetGameState())->CompeleteObjectiveCall(FName("0002"), true);
			}
			HGCharacter->FlashLightComponent->AddBatteryLife(BatteryAmount);
			//GEngine->AddOnScreenDebugMessage(99, 5, FColor::Green, FString("use Battery"), true);
			
			
		}
		else {
			return;
		}
	}
	Super::UseItem();
}

void AFlashLightBattery::ItemPickUp()
{
	Super::ItemPickUp();
	if (IsActorBeingDestroyed()) {
		Cast<ALl1_GameStateBase>(GetWorld()->GetGameState())->CompeleteObjectiveCall(FName("0001"), true);
		UE_LOG(LogTemp, Display, TEXT("0000destroy"));
	}

}


