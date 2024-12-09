// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Items/Bandage.h"
#include "Character/AttributeComponent.h"
#include "Character/HGCharacter.h"
ABandage::ABandage()
{
	ItemID = "B_0002";
	AddHealthAmount = 50.f;
}

void ABandage::UseItem()
{
	if (HGCharacter) {
		if (HGCharacter->AttributeComponent->bIsHealthNotFull()) {
			HGCharacter->AttributeComponent->AddHealth(AddHealthAmount);
			
			//AHGCharacter* Testddd= Cast<AHGCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());//Copy한 아이템이 이거지.
			//Testddd->AttributeComponent->AddHealth(AddHealthAmount);
		}
		else {
			return;
		}
	}
	Super::UseItem();
}
