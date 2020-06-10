// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CeoDepMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTIDLE_API UCeoDepMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	class UGameManager* GM;

	UPROPERTY(BlueprintReadWrite) class AOfficeDepartment* OfficeDepartment;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UScrollBox* IdeaScrollBox;

	UPROPERTY(EditAnywhere) TSubclassOf<UUserWidget> IdeaButtonWidgetClass;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* PublishGame_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Hire_Prog_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Hire_Artist_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Hire_ProgSup_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UButton* Hire_ArtistSup_Btn;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget)) class UTextBlock* Total_Artist_Btn;
	UPROPERTY() FString Worker;

	int Index = 0;
	int ChosenIndex;

public:
	virtual void NativeConstruct() override;
	void GetFinishedIdea(class Idea* idea);

private:
	UFUNCTION() void CallHiring();
	UFUNCTION() void CallProgrammerSpawn();
	UFUNCTION() void CallProgrammerSupSpawn();
	UFUNCTION() void CallArtistSpawn();
	UFUNCTION() void CallArtistSupSpawn();
	UFUNCTION() void ActiveWorkstation(int Number);
	UFUNCTION() void PublishGame();
	
	void AddValuesToButton(class Idea* idea);
};
