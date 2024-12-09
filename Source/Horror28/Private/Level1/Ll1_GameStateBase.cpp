// Fill out your copyright notice in the Description page of Project Settings.


#include "Level1/Ll1_GameStateBase.h"
#include "HUD/MainWidget.h"
#include "HUD/Interface/ObjectivesWidget.h"
#include "Components/TextBlock.h"
void ALl1_GameStateBase::HandleBeginPlay()
{
	Super::HandleBeginPlay();
	
	//FindCurrentObjective();
	//CompleteObjective();
	//FindCurrentObjective();
	//CompleteObjective();
	//FindCurrentObjective();
}

void ALl1_GameStateBase::FindCurrentObjective()
{
	TArray<FObjectivesData*> AllRows;
	ObjectivesDataTable->GetAllRows(FString(""), AllRows);
	for (FObjectivesData* QuestData : AllRows) {
		//UE_LOG(LogTemp, Warning, TEXT("Objective Name: %s"), *QuestData->IDName.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Objective Description: %s"), *QuestData->ObjectivesTitle.ToString());
		
		if (QuestData->bIsObjectiveCompleted) {
			UE_LOG(LogTemp, Warning, TEXT("Objective true"));
			
			//return ObjectivesData;
		}
		else {
			ObjectivesData = QuestData;
			UE_LOG(LogTemp, Warning, TEXT("Objective false"));
			return;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("--------------------------------------"));
	//UE_LOG(LogTemp, Warning, TEXT("--------------------------------------"));
}

void ALl1_GameStateBase::Initialize(UMainWidget* MainWidget)
{
	MainWidgetRef = MainWidget;
	SetObjective();
}

void ALl1_GameStateBase::SetObjective()
{
	FindCurrentObjective();
	MainWidgetRef->ObjectivesWidget->UpdateObjectiveText(ObjectivesData->ObjectivesTitle);
	MainWidgetRef->ObjectivesWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void ALl1_GameStateBase::CompleteObjective()
{
	//TArray<FObjectivesData*> AllRows;
	//ObjectivesDataTable->GetAllRows(FString(""), AllRows);
	//for (FObjectivesData* QuestData : AllRows) {
		
	//}


	ObjectivesData->bIsObjectiveCompleted = true;
	MainWidgetRef->ObjectivesWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void ALl1_GameStateBase::CompeleteObjectiveCall(FName ObjectiveID, bool bIsSetNextObjective)
{
	//ObjectivesDataTable->FindRow(,)
	
	FObjectivesData* Data =ObjectivesDataTable->FindRow<FObjectivesData>(ObjectiveID,ObjectiveID.ToString());
	//Data->bIsObjectiveCompleted = true;
	if (Data->IDName == ObjectivesData->IDName) {
		CompleteObjective();
		if (bIsSetNextObjective) {
			SetObjective();
		}
	}
	
}
