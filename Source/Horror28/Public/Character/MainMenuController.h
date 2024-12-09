// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuController.generated.h"

class UMainMenuWidget;
/**
 * 
 */
UCLASS()
class HORROR28_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
public:
	UMainMenuWidget* MainMenuWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf< UMainMenuWidget> MainMenuWidgetClass;
	virtual void BeginPlay() override;
};
