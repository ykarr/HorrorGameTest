// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Items/DoorKey.h"

ADoorKey::ADoorKey()
{
	ItemID = "K_0001";
}

void ADoorKey::BeginPlay()
{
	Super::BeginPlay();
}

void ADoorKey::UseItem()
{
	UE_LOG(LogTemp, Display, TEXT("Use Key"));
	
	//if (ItemQuantity < 1) {
	//	Destroy();
	//}
	
	
	Super::UseItem();
}
