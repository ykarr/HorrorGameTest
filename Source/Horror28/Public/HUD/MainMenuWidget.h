// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class HORROR28_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized();
	UPROPERTY(meta = (BindWidget))
	UButton* PlayBtn;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitBtn;
	UPROPERTY(EditAnywhere, Category = "Level")
	ULevel* InGameLevel;
	UFUNCTION()
	void GameStartBtn();
	UFUNCTION()
	void GameQuitBtn();
};
