// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Interface/ArrowWidget.h"
#include "Components/Button.h"
#include "Actor/Items/LockDial.h"



void UArrowWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	ArrowBtn->OnPressed.AddDynamic(this, &UArrowWidget::OnUpPressBtn);
}

void UArrowWidget::OnUpPressBtn()
{
	if (bIsAddRot) {
		LockDialRef->RotDial();
	}
	else {
		LockDialRef->RotDialDown();
	}
}
