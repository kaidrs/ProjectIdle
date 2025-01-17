// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ProjectIdleCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Materials/Material.h"
#include "GameUI.h"
#include "GameHUD.h"
#include "Engine/World.h"
#include "GameManager.h"
#include "ProjectIdle/GameHUD.h"

AProjectIdleCharacter::AProjectIdleCharacter()
{
	//TotalMoney = 1000;

	//MoneyText->SetText(FText::FromString("Money: " + FString::FromInt(Money)));

	//FTxtMoney = FString("Money: " + FString::FromInt(Money));

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AProjectIdleCharacter::BeginPlay()
{
	Super::BeginPlay();
	GM = GetWorld()->GetGameInstance<UGameManager>();
	GM->Character = this;
	PlayerControl = Cast<APlayerController>(GetController());
	PlayerControl->GetViewportSize(ScreenSizeX, ScreenSizeY);

	auto Widget = CreateWidget<UGameUI>(UGameplayStatics::GetPlayerController(this, 0), WidgetClass);
	if (Widget)
	{
		Widget->player = this;
		Widget->AddToViewport();
	}

	AGameHUD* GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void AProjectIdleCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	PlayerControl->GetViewportSize(ScreenSizeX, ScreenSizeY);


	//AddActorWorldOffset(GetCameraPanDirection() * 2);
	//CameraBoom->AddWorldOffset(GetCameraPanDirection() * CameraMovementSpeed);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params(NAME_None, FCollisionQueryParams::GetUnknownStatId());
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
	//TopDownCameraComponent->AddWorldOffset(GetCameraPanDirection() * CameraMovementSpeed);
}


void AProjectIdleCharacter::PlayHelpAnim()
{
	if (HelpAnimation != nullptr) {
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Play(HelpAnimation, 1.f);
		FTimerHandle DelayHandle;
		GetWorldTimerManager().SetTimer(DelayHandle, this, &AProjectIdleCharacter::ResumeMovement, 4.25f, false);
		GetCharacterMovement()->DisableMovement();
		//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	}
}

void AProjectIdleCharacter::ResumeMovement() {
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

}

FVector AProjectIdleCharacter::GetCameraPanDirection()
{
	float MousePositionX;
	float MousePositionY;
	float CamDirectionX = 0;
	float CamDirectionY = 0;

	if (PlayerControl != nullptr)
	{
		PlayerControl->GetMousePosition(MousePositionX, MousePositionY);
		FString mouseX = FString::FromInt(MousePositionX);
		FString mouseY = FString::FromInt(MousePositionY);
		UE_LOG(LogActor, Warning, TEXT("%s"), *mouseX)
			UE_LOG(LogActor, Warning, TEXT("%s"), *mouseY)

	}
	if (MousePositionX >= 1 && MousePositionX <= 20)
	{
		CamDirectionY = -1;
	}
	if (MousePositionY <= 1)
	{
		CamDirectionX = 1;
	}
	if (MousePositionX >= ScreenSizeX - 15)
	{
		CamDirectionY = 1;
	}
	if (MousePositionY >= ScreenSizeY - 15)
	{
		CamDirectionX = -1;
	}

	return FVector(CamDirectionX, CamDirectionY, 0);
}
