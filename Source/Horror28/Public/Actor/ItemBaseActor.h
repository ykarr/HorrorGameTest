// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor/ItemData/ItemDataTable.h"
#include "ItemBaseActor.generated.h"

//struct FItemData;
class UDataTable;
class USphereComponent;
class UStaticMeshComponent;
class UInventoryComponent;
class UWidgetComponent;
class UPickupWidget;
class UEnhancedInputComponent;
class UInputAction;
class AHGCharacter;

UCLASS()
class HORROR28_API AItemBaseActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UWorld* World;
	UPROPERTY(EditAnywhere, Category = "ItemData | Result")
	FText ItemName; //Just Item ID
	UPROPERTY(EditAnywhere, Category = "ItemData | Result")
	bool bItemStackable; 
	UPROPERTY(EditAnywhere, Category = "ItemData | Result")
	bool bIsCanUseable;
	UPROPERTY(EditAnywhere, Category = "ItemData | Result")
	int32 MaxStackData;
	UPROPERTY(EditAnywhere,Category="ItemData")
	int32 ItemQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	UPROPERTY(EditDefaultsOnly, Category = "Item | Component")
	TObjectPtr<USphereComponent> ItemSphere;
	UInventoryComponent* InventoryRef;
	int32 index;
	TSubclassOf<AItemBaseActor> ItemBaseActorClass;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> PickupWidgetComponent;
	TObjectPtr<UPickupWidget> ItemPickupWidget; 

	/** Fire Input Action */


	virtual void UseItem();
	AItemBaseActor* CreateItemCopy() const;
	virtual void ItemPickUp();
	void AddPickupContext();
	void FreePickupContext();
	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName ItemID;
protected:
	AItemBaseActor();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/*Find Item by Item ID */
	
	UPROPERTY(EditAnywhere, Category = "ItemData")
	TObjectPtr<UDataTable> ItemDataTable;
	
	FItemData* ItemData;
	AHGCharacter* HGCharacter;
private:
	void InitalizedPickup();
	//USceneComponent* SceneRoot;
	UFUNCTION()
	void OnItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InputAction;
	bool bIsUpdateActor;
	float PickupDistance;
	bool bIsCanPickup;

public:	
	FORCEINLINE FItemData* GetItemData() const { return ItemData; }
	FORCEINLINE bool IsFullItemStack() const { return ItemQuantity >= MaxStackData; }
	

};
