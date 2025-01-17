// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Employee.h"
#include "Supervisor.generated.h"

UCLASS()
class PROJECTIDLE_API ASupervisor : public AEmployee
{
	GENERATED_BODY()

public:
	//class UGameManager* GM;
	// Sets default values for this character's properties
	ASupervisor(ERole Department);
	ASupervisor();

	UPROPERTY(SaveGame) bool IsEvaluating;
	UPROPERTY(SaveGame) AEmployee* EmployeeToEval;

	bool CheckingForHelp;
	//FTimerHandle HelpTimer;


	void InitSupervisor(ERole Department);
	void EvaluateEmployee();
	UFUNCTION() void CheckForHelp(); //Called by timer
	AEmployee* TargetEmployee;
	virtual void FiredFinal() override;
	void BeginSupervisorWork();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
