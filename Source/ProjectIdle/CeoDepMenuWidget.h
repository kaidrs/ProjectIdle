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
	UPROPERTY(BlueprintReadWrite) class AOfficeDepartment* OfficeDepartment;
	virtual void NativeConstruct() override;
};
