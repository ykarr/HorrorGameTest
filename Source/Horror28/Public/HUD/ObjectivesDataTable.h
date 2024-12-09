// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ObjectivesDataTable.generated.h"

USTRUCT()
struct FObjectivesData :public FTableRowBase {
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditDefaultsOnly)
	FName IDName;
	UPROPERTY(EditDefaultsOnly)
	FText ObjectivesTitle;
	UPROPERTY(EditDefaultsOnly)
	bool bIsObjectiveCompleted;
};
/**
 * 
 */
UCLASS()
class HORROR28_API UObjectivesDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
