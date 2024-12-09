// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Examination.generated.h"

class UStaticMeshComponent;
UCLASS()
class HORROR28_API AExamination : public AActor
{
	GENERATED_BODY()
	
public:	
	void ResetRot();
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	TObjectPtr< UStaticMeshComponent> ItemMesh;
protected:
	AExamination();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	

private:
	FTransform SaveTransform;
};
