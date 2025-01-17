// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "WorkstationUpgradeWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTIDLE_API UWorkstationUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class AWorkstation* Station;
	class UGameManager* GM;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UScrollBox* InventoryScrollBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UWrapBox* InventoryWrapBox;
	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> InventoryButtonWidgetClass;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Monitor_Btn; //change to respective categories
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Keyboard_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Desk_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Chair_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* ReturnUpgrBtn;

	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Keyboard_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UImage* MonitorImage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UImage* Desk_Img;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UImage* Chair_Img;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UImage* Keyboard_Img;
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UImage* KeyboardImage;
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* UpgradeMonitor_Btn;
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* UpgradeKeyboard_Btn;

public:
	UFUNCTION(BlueprintCallable, Category = "CPP Functions") void ShowInventory(ESubCategory ItemCategory);
	UFUNCTION(BlueprintCallable, Category = "CPP Functions") void ShowInventoryMonitor();
	UFUNCTION(BlueprintCallable, Category = "CPP Functions") void ShowInventoryDesk();
	UFUNCTION(BlueprintCallable, Category = "CPP Functions") void ShowInventoryChair();
	UFUNCTION(BlueprintCallable, Category = "CPP Functions") void ShowInventoryKeyboard();
	UFUNCTION() void CloseUpgrWidget();

private:
	virtual void NativeConstruct() override;
	
};