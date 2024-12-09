// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryBackPack.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;
class UPickupWidget;
class AHGCharacter;
UCLASS()
class HORROR28_API AInventoryBackPack : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "Backpack")
	int32 AmountOfSlots;
	UPROPERTY(EditAnywhere, Category = "Backpack")
	UStaticMeshComponent* BackPackMesh;
	UPROPERTY(EditAnywhere, Category = "Backpack")
	USphereComponent* Sphere;
	UPROPERTY(EditAnywhere, Category = "Backpack")
	TObjectPtr<UWidgetComponent> BaseWidgetComponent;
	TObjectPtr<UPickupWidget> PickupWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InputAction;
protected:
	AInventoryBackPack();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void BackPackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void BackPackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	bool bIsUpdateActor;
	AHGCharacter* HGCharacter;
	float PickupDistance;
private:
	void AddPickUpContext();
	void FreePickUpContext();
	void PickupItem();
	bool bIsCanPickup;
public:	

};
