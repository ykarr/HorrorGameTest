// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainMenuController.h"
#include "HUD/MainMenuWidget.h"
#include "Kismet\GameplayStatics.h"
#include "Camera/CameraActor.h"
void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeUIOnly InputMode;
	this->SetInputMode(InputMode);
	this->SetShowMouseCursor(true);
	MainMenuWidget = CreateWidget< UMainMenuWidget>(this, MainMenuWidgetClass);
	if (MainMenuWidget) {
		UE_LOG(LogTemp, Display, TEXT("Play"));
		MainMenuWidget->AddToViewport(10);
	}
	AActor* targetActor=UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass());
	SetViewTargetWithBlend(targetActor);
	//UMainMenuWidget* MainMenuWidget = Cast<UMainMenuWidget>(CreateWidget(this, UMainMenuWidget::StaticClass()));
	
}
