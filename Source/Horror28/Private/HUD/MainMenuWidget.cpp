// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
void UMainMenuWidget::NativeOnInitialized()
{
	PlayBtn->OnPressed.AddDynamic(this, &UMainMenuWidget::GameStartBtn);
	QuitBtn->OnPressed.AddDynamic(this, &UMainMenuWidget::GameQuitBtn);
}

void UMainMenuWidget::GameStartBtn()
{
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	//UKismetSystemLibrary::PrintString(GetWorld());
	UGameplayStatics::OpenLevel(GetWorld(),FName("/Game/Maps/Level1-1"));
	//UGameplayStatics::OpenLevel(this,FName("/Game/Maps/Level1-1"));

}

void UMainMenuWidget::GameQuitBtn()
{
	//UGameplayStatics::
	UKismetSystemLibrary::QuitGame(GetWorld(),nullptr,EQuitPreference::Quit,false);
}
