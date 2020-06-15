// Fill out your copyright notice in the Description page of Project Settings.


#include "Workstation.h"
#include "ProjectIdle/GameManager.h"
#include "Employees/Artist.h"
#include "Employees/Programmer.h"
#include "ProjectIdle/GameHUD.h"
#include "Workstations/ArtistStation.h"
#include "Workstations/ProgrammerStation.h"
#include "ProjectIdle/Widgets/WorkstationUpgradeWidget.h"
#include "ProjectIdle/Widgets/WorkstationCompileWidget.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine.h"

// Sets default values
AWorkstation::AWorkstation()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DeskMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DeskMesh"));
	ComputerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ComputerMesh"));
	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh"));
	KeyboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyboardMesh"));
	RootComponent = DeskMesh;

	StationRole = ERole::Programmer;//Default to stop crashing
	UpgradeMonitor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpgradeMonitor"));
	UpgradeKeyboard = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpgradeKeyBoard"));
	//UpgradeChair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UpgradeChair"));

	UpgradeMonitor->SetupAttachment(RootComponent);
	UpgradeKeyboard->SetupAttachment(RootComponent);
	ComputerMesh->SetupAttachment(RootComponent);
	ChairMesh->SetupAttachment(RootComponent);
	KeyboardMesh->SetupAttachment(RootComponent);

	CompileProgressBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("WorkloadProgressBar")); //Maybe make Employee BP to set this up, because if later Employee classes emerge if we
	CompileProgressBar->AttachTo(RootComponent);
	CompileProgressBar->SetVisibility(false);
	CompileProgressBar->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

}

// Called when the game starts or when spawned
void AWorkstation::BeginPlay()
{
	Super::BeginPlay();
	//IsObjectDisable = DisableObject;
	EnableStation(IsEnabled);
	GM = GetWorld()->GetGameInstance<UGameManager>();
	GM->WorkstationList.Add(this);
	HasEmployee = false;
	StationLocation = ChairMesh->GetComponentLocation();
	UI = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(this->GetOwner(), 0));

	if (UserWidget != nullptr)
	{
		UpgradeWidget = CreateWidget<UWorkstationUpgradeWidget>(UGameplayStatics::GetPlayerController(this, 0), UserWidget);
	}
	if (UpgradeWidget)
	{
		UpgradeWidget->Station = this;
	}

	Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (CompileProgressBar != nullptr) {
		auto WorkloadWidget = CompileProgressBar->GetUserWidgetObject();
		auto WidgetInstance = Cast<UWorkstationCompileWidget>(WorkloadWidget);
		WidgetInstance->WorkstationRef = this;
		CompileProgressBar->SetVisibility(false);
	}

	//DoCompile();//test
}

// Called every frame
void AWorkstation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//DisableStation(DisableObject);

	//Same as employee workload diminish, move to event/timer delegate function later
	if (IsCompiling && IsEnabled) {
		if (CompileProgressBar != nullptr) {
			CompileProgressBar->SetWorldRotation(Camera->GetCameraRotation());
			CompileProgressBar->AddLocalRotation(FRotator(0, 180, 0));
		}
		if (CurrentCompileLoad > 0) {
			CurrentCompileLoad -= (DeltaTime * (5.f + CompileModifier));
			if (CurrentCompileLoad <= 0) {
				CompileProgressBar->SetVisibility(false);
				IsCompiling = false;
				//send to employee that yolo etc
			}
		}

	}
}

void AWorkstation::UpdateWorkstationPosition()
{
	if (!HasEmployee) {
		int32 employeeSize = GM->EmployeeList.Num();
		int32 workstationSize = GM->WorkstationList.Num();
		FVector AStationLocation = this->GetActorLocation();

		for (auto Employee : GM->EmployeeList) {
			if (!Employee->HasWorkStation && Employee->EmployeeRole == StationRole && IsEnabled) {
				Employee->WorkstationRef = this;
				Employee->HasWorkStation = true;
				Employee->StartPosition = StationLocation;
				HasEmployee = true;
			}
		}
	}
}

void AWorkstation::EnableStation(bool Enable)
{
	if (!Enable)
	{
		this->SetActorHiddenInGame(true);
		this->SetActorEnableCollision(false);
		//this->SetActorTickEnabled(true);
		IsEnabled = false;
	}
	else
	{
		this->SetActorHiddenInGame(false);
		this->SetActorEnableCollision(true);
		//this->SetActorTickEnabled(false);
		IsEnabled = true;
	}
}


void AWorkstation::NotifyActorOnClicked(FKey ButtonPressed)
{
	if (!UpgradeWidget->IsInViewport())
	{
		UpgradeWidget->AddToViewport();
	}
	else
	{
		UpgradeWidget->RemoveFromViewport();
	}
}

void AWorkstation::UpgradeMesh(int Index)
{
	if (Index == 0)
	{
		ComputerMesh->SetVisibility(false);
		UpgradeMonitor->SetVisibility(true);
		CompileModifier += 10;
	}
	if (Index == 1)
	{
		KeyboardMesh->SetVisibility(false);
		UpgradeKeyboard->SetVisibility(true);
		CompileModifier += 5;
	}
	UpgradeWidget->RemoveFromViewport();
}

void AWorkstation::DoCompile() {
	IsCompiling = true;
	CompileProgressBar->SetVisibility(true);

	//Make function for generation of compile loads scaling with player progress, etc
	AssignedCompileLoad = UKismetMathLibrary::RandomFloatInRange(50, 100);
	CurrentCompileLoad = AssignedCompileLoad;

}