// Fill out your copyright notice in the Description page of Project Settings.


#include "Level1/L1HGHUD.h"
#include "HUD/MainWidget.h"
#include "HUD/Inventory/InventoryMenu.h"
#include "HUD/Inventory/InventoryGrid.h"
AL1HGHUD::AL1HGHUD()
{


}

void AL1HGHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (UWorld* World = GetWorld()) {
		if (APlayerController* controller = World->GetFirstPlayerController()) {
			if (controller) {
				if (MainWidgetClass) {
					MainWidget = CreateWidget<UMainWidget>(controller, MainWidgetClass); 
					MainWidget->AddToViewport(1);
				}
				if (MainWidgetClass) {
					InventoryMain = CreateWidget<UInventoryMenu>(controller, InventoryMainClass);
					InventoryMain->AddToViewport(0);
					InventoryMain->SetVisibility(ESlateVisibility::Collapsed);
					//InventoryMain->InventoryGrid->InitSlots();
				}
			}
		}
	}
}

void AL1HGHUD::HideInventory()
{
	if (InventoryMain) {
		InventoryMain->SetVisibility(ESlateVisibility::Collapsed);
		InventoryMain->HideDropDownBtn();
		//InventoryMain->HideCloseDropBtn();
	}
}

void AL1HGHUD::DisplayInventory()
{
	if (InventoryMain) {
		InventoryMain->SetVisibility(ESlateVisibility::Visible);
		//InventoryMain->DisplayCloseDropBtn();
		//InventoryMain->DisplayDropDownBtn();
	}
}

/*
// Example code to set the progress bar value
// You can call this function from anywhere in your code
void SetProgressBarValueFromAnywhere(float NewValue)
{
	// Get a reference to the player controller
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		// Get a reference to the widget
		UMyWidget* MyWidget = Cast<UMyWidget>(PlayerController->GetHUD()->GetUserWidgetObject());

		if (MyWidget)
		{
			// Call the function to set the progress bar value
			MyWidget->SetProgressBarValue(NewValue);
		}
	}
}*/