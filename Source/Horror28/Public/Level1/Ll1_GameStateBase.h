// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HUD/ObjectivesDataTable.h"
#include "Interface/ObjectiveCallInterface.h"
#include "Ll1_GameStateBase.generated.h"

class UMainWidget;
/**
 * 
 */
UCLASS()
class HORROR28_API ALl1_GameStateBase : public AGameStateBase,public IObjectiveCallInterface
{
	GENERATED_BODY()
public:
	virtual void HandleBeginPlay() override;
	void FindCurrentObjective();
	void Initialize(UMainWidget* MainWidget);
	void SetObjective();
	void CompleteObjective();
	/*Interface Function*/
	virtual void CompeleteObjectiveCall(FName ObjectiveID, bool bIsSetNextObjective)override;
	UPROPERTY(EditAnywhere, Category = "ObjectivesData")
	TObjectPtr<UDataTable> ObjectivesDataTable;
	FObjectivesData* ObjectivesData;
	UMainWidget* MainWidgetRef;
	//UPROPERTY(EditAnywhere, Category = "ItemData")
	//TObjectPtr<UDataTable> ItemDataTable;
	//TObjectPtr<FItemData> ItemData;
};
