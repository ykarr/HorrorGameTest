// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/GameOverWidget.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UGameOverWidget::NativeOnInitialized()
{
	RestartBtn->OnPressed.AddDynamic(this, &UGameOverWidget::RestartGame);
}
void UGameOverWidget::NativeConstruct()
{
	//FaidInIn = GetAnimation
	//FaidInIn=AnimationMap.
	PlayAnimation(FaidIn);
}

void UGameOverWidget::RestartGame()
{
	ULevel* level= GetWorld()->GetCurrentLevel();
	UGameplayStatics::OpenLevel(GetWorld(), level->GetFName());
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
}
