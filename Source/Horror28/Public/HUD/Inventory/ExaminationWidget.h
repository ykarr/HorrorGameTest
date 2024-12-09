// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExaminationWidget.generated.h"


//DECLARE_MULTICAST_DELEGATE(FOnReturnUpdate);
class UTextBlock;
class UImage;
class UInventoryComponent;
class UInputAction;
/**
 * 
 */
UCLASS()
class HORROR28_API UExaminationWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	//#include "Components/Image.h"
	UPROPERTY(meta = (BindWidget))
	UImage* DragDetection;
	//#include "Components/TextBlock.h"
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemDescriptionText;

	UInventoryComponent* InventoryComponentRef;
	void UpdateWidget(int32 Index);
	//UPROPERTY(EditDefaultsOnly, Category = "Input | Move")
	//TObjectPtr<UInputAction> ReturnAction;
protected:
	virtual void NativeConstruct();
	//void StartReturnFunc();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	FVector2D MouseLoc;
	bool bIsRotateMesh;
	UStaticMeshComponent* Mesh;
};
