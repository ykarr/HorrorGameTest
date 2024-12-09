// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/MainWidget.h"
#include "HUD/Interface/ObjectivesWidget.h"
#include "Components/TextBlock.h"


void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ObjectivesWidget) {
		ObjectivesWidget->ObjectiveText->SetText(FText::FromString("Objective"));
	}
}
