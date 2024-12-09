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
NativeOnInitialized : ������ ������ �� �� �� �� ȣ��ȴ�. ������ ���� �ÿ��� ������ �� ȣ��ȴ�.
NativeConstruct : AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!
NativeDestruct : RemoveFromParent(RemoveFromViewport) �� ȣ��ȴ�. Viewport ���� Remove �� ������ �Ҹ���!
*/