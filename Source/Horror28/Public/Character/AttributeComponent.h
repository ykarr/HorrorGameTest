// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "Materials/MaterialParameterCollection.h"
#include "AttributeComponent.generated.h"

class UAttributeProgressWidget;
class UMaterialParameterCollection;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HORROR28_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	void DebugDamage(float bIsDamaged);
	void SetHealthBarWidget();
	void RemoveHealth(float Amount);
	void AddHealth(float Amount);
	bool bIsHealthNotFull();
	float GetHealthBarPercent();

	TObjectPtr<UAttributeProgressWidget> AttributeProgressWidget;
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	UMaterialParameterCollection* HealthParameterCollection;
	
protected:

	UAttributeComponent();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	void UpdateBloodScreen();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	float MinHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, Category = "Attribute")
	float MaxHealth;
	

	

};
