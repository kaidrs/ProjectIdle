// Fill out your copyright notice in the Description page of Project Settings.


#include "ProgrammingDepartment.h"
#include "ProjectIdle/GameManager.h"

AProgrammingDepartment::AProgrammingDepartment() {
	DepartmentCapacity = 3;
}

void AProgrammingDepartment::BeginPlay() {
	Super::BeginPlay();
	GM = GetWorld()->GetGameInstance<UGameManager>();
	DepRole = ERole::Programmer;
	GM->ProgrammingDepartment = this;
	GM->DepartmentList.Add(this);
}