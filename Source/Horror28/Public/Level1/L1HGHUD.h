// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "L1HGHUD.generated.h"

class UMainWidget;
class UInventoryMenu;
class UInventoryGrid;
/**
 * 
 */
UCLASS()
class HORROR28_API AL1HGHUD : public AHUD
{
	GENERATED_BODY()
public:
	AL1HGHUD();
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UMainWidget> MainWidgetClass;
	TObjectPtr<UMainWidget> MainWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UInventoryMenu> InventoryMainClass;
	TObjectPtr<UInventoryMenu> InventoryMain;
	
	void HideInventory();
	void DisplayInventory();
};
