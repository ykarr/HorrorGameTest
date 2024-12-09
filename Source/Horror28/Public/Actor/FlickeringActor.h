// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlickeringActor.generated.h"

class UPointLightComponent;
//struct FTimerHandle;
UCLASS()
class HORROR28_API AFlickeringActor : public AActor
{
	GENERATED_BODY()
	
public:	
	float DelayTime;
protected:
	AFlickeringActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "FlickeringLight")
	UPointLightComponent* FlickeringLight;
	UPROPERTY(EditAnywhere, Category = "FlickeringLight")
	float Intensity;
	
	FTimerHandle DelayTimerHandle;
	
private:
	UFUNCTION()
	void LightFlickering();
	bool bIsMaxLight;

};
