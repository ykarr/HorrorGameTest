// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"
class AItemBaseActor;

USTRUCT()
struct FItemData: public FTableRowBase {
	GENERATED_USTRUCT_BODY()
	FItemData() :
		bIsCanExamination(true),
		bIsCanDropable(true),
		bIsStackable(true),
		bIsCanUseable(true),
		MaxStackAmount(10)
	{};
	/*Examination*/
	UPROPERTY(EditAnywhere)
	UStaticMesh* ExaminationMesh;
	UPROPERTY(EditAnywhere)
	float ExaminationMeshOffset;
	UPROPERTY(EditAnywhere)
	FRotator ExaminationMeshRotateOffset;
	UPROPERTY(EditAnywhere)
	bool bIsCanExamination;
	/*Examination End*/
	UPROPERTY(EditAnywhere)
	bool bIsCanDropable;
	UPROPERTY(EditAnywhere)
	bool bIsStackable;
	UPROPERTY(EditAnywhere)
	bool bIsCanUseable;
	UPROPERTY(EditAnywhere)
	FText ItemName;
	UPROPERTY(EditAnywhere)
	UTexture2D* ItemIcon;
	UPROPERTY(EditAnywhere)
	FText ItemDescription;
	UPROPERTY(EditAnywhere)
	int32 MaxStackAmount;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AItemBaseActor> BaseActor;
};
//USTRUCT()
//struct FItemData {
//	GENERATED_USTRUCT_BODY()
//	UPROPERTY(EditAnywhere)
//	FName ItemName;
//	UPROPERTY(EditAnywhere)
//	UTexture2D* ItemIcon;
//};
/**
 * 
 */
UCLASS()
class HORROR28_API UItemDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
