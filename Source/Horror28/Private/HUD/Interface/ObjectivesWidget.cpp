// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/Interface/ObjectivesWidget.h"
#include "Components/TextBlock.h"
void UObjectivesWidget::UpdateObjectiveText(FText InText)
{
	ObjectiveText->SetText(InText);
}
