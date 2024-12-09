// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UWidgetAnimation;
class UButton;
/**
 * 
 */
UCLASS()
class HORROR28_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized()override;
	virtual void NativeConstruct() override; //Con
	UPROPERTY(BlueprintReadWrite,Category = "UI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FaidIn;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartBtn;
	//UPROPERTY(BlueprintReadOnly, Category = "BindAnimation", meta = (BindWidgetAnim), Transient)
	//UWidgetAnimation* OpenAnim;
	UFUNCTION()
	void RestartGame();
};


/*
NativeOnInitialized : 위젯이 생성될 때 딱 한 번 호출된다. 에디터 편집 시에도 생성될 때 호출된다.
NativeConstruct : AddToViewport 시 호출된다. NativeOnInitialized 와 달리, Viewport 에 Add 될 때마다 불린다!
NativeDestruct : RemoveFromParent(RemoveFromViewport) 시 호출된다. Viewport 에서 Remove 될 때마다 불린다!
*/