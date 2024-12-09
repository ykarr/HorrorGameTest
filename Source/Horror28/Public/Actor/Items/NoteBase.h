// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NoteBase.generated.h"

class UTextRenderComponent;
class USphereComponent;
class UWidgetComponent;
class UPickupWidget;
class AHGCharacter;
class UInputAction;
class UNoteExaminationWidget;
class AExaminationNote;
UCLASS()
class HORROR28_API ANoteBase : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> PickupWidgetComponent;
	TObjectPtr<UPickupWidget> ItemPickupWidget;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PaperMesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr <UTextRenderComponent> TextRender;
	UPROPERTY(EditAnywhere)
	TObjectPtr <UTextRenderComponent> BackTextRender;
	UPROPERTY(EditAnywhere)
	FText NoteText;
	UPROPERTY(EditAnywhere)
	FText BackNoteText;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> OverlapSphereComponent;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UNoteExaminationWidget> NoteExaminationWidgetClass;
	TObjectPtr<UNoteExaminationWidget> NoteExaminationWidget;

protected:
	// Called when the game starts or when spawned
	ANoteBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void AddPickupContext();
	void FreePickupContext();
	void ViewExamination();
	UPROPERTY(EditDefaultsOnly)
	float PickupDistance;
	bool bIsUpdateActor;
	bool bIsCanPickup;
	TObjectPtr<AHGCharacter> HGCharacter;
	TObjectPtr<AExaminationNote> ExaminationNoteRef;
};
