// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemButton.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTIDLE_API UItemButton : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemButton(const FObjectInitializer& ObjectInitializer);

	class UGameManager* GameManager;

	UPROPERTY(meta = (BindWidget)) class UButton* Item_Btn;
	UPROPERTY(meta = (BindWidget)) class UImage* Item_I;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ItemName_T;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ItemPrice_T;
	UPROPERTY(meta = (BindWidget)) class UTextBlock* ItemCount_T;
	UPROPERTY(EditAnywhere) TSubclassOf<class AItem> BPItem;

	class AItem* Item;

	int ItemID;
	int ItemCount = 1;
	bool InCheckout = false;

private:
	void NativeConstruct() override;

	UFUNCTION() void OnClicked();
};