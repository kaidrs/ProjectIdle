// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "ProjectIdle/Shop/Item.h"
#include "ProjectIdle/InteractableObject.h"
#include "Components/WidgetComponent.h"
#include "ProjectIdle/Employees/Employee.h"
#include "Workstation.generated.h"

enum class EType : int16
{
	PROGRAMMER,
	ARTIST
};


//USTRUCT()
//struct FSaveMesh {
//	GENERATED_USTRUCT_BODY()
//	int WorkstationIndex;
//	int S_ComputerMeshID;
//};

UCLASS()
class PROJECTIDLE_API AWorkstation : public AActor, public IInteractableObject
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AWorkstation();

	bool InRange;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* CollisionBox;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite) int ComputerMeshID;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite) int ChairMeshID;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite) int DeskMeshID;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite) int KeyboardMeshID;

	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite) int WorkstationIndex;//for now set in editor, make dynamic later
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite) float AssignedCompileLoad;
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite) float CurrentCompileLoad;

	UPROPERTY(SaveGame, VisibleAnywhere)
		UStaticMeshComponent* DeskMesh;
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* ComputerMesh;
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* ChairMesh;
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* KeyboardMesh;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Role") ERole StationRole;
	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, Category = "Role") EPosition StationOwnerPosition;

	UPROPERTY()
		FVector StationLocation;

	UPROPERTY()
		FVector StationVector;

	UPROPERTY() bool IsCompiling;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool HasEmployee;
	UPROPERTY(SaveGame, EditAnywhere, BlueprintReadWrite) bool IsEnabled;

	APlayerCameraManager* Camera;
	class UGameManager* GM;
	class AGameHUD* UI;
	//EType type;

	UPROPERTY(EditAnywhere, Category = "Widgets") TSubclassOf<UUserWidget> UserWidget;
	UPROPERTY() class UWorkstationUpgradeWidget* UpgradeWidget;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) UWidgetComponent* CompileProgressBar;
	UPROPERTY(SaveGame) int CompileModifier = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor") int FloorLevel = -1;


private:
	/*UPROPERTY(EditAnywhere) TSubclassOf<AItem> DefaultMonitor;
	UPROPERTY(EditAnywhere) TSubclassOf<AItem> DefaultKeyboard;
	UPROPERTY(EditAnywhere) TSubclassOf<AItem> DefaultDesk;
	UPROPERTY(EditAnywhere) TSubclassOf<AItem> DefaultChair;

	AItem* CurrentMonitor;
	AItem* CurrentKeyboard;
	AItem* CurrentDesk;
	AItem* CurrentChair;*/

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void NotifyActorOnClicked(FKey ButtonPressed = EKeys::RightMouseButton) override;

	UFUNCTION() void UpdateWorkstationPosition(AEmployee* EmployeeRef);
	UFUNCTION() void UpdateSupervisorWorkstationPosition();

	UFUNCTION(BlueprintCallable) void EnableStation(bool Enabled);
	UFUNCTION() void DoCompile();
	void UpgradeMesh(AItem* Item);
	void UpgradeMeshFromSave();
	//UFUNCTION(BlueprintCallable) void InitFromSave(int Index);

	//UFUNCTION(BlueprintCallable) void UpgradeMeshFromSave(AWorkstation* SavedStation);
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
	//UFUNCTION() int WorkstationActiveLenght();
	//UFUNCTION() void TestFunction();
	void WorkstationInit(ERole StationRole);

	// Inherited via IInteractableObject
	virtual void OnInteract();
};
