// Fill out your copyright notice in the Description page of Project Settings.


#include "MeetingDepartment.h"
#include "Idea.h"
#include "Components/WidgetComponent.h"
#include "Widgets/MeetingDepWidget.h"
#include "EngineUtils.h"


// Sets default values
AMeetingDepartment::AMeetingDepartment()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	OfficeLocation = FVector(-720.0, 900, 200);

}


// Called when the game starts or when spawned
void AMeetingDepartment::BeginPlay()
{
	Super::BeginPlay();
	GM = GetWorld()->GetGameInstance<UGameManager>();
	GM->MeetingDepartment = this;
	//temp assign, need to change class name , maybe move functions to gm
	UWorld* world = GetWorld();
	if (UserWidget != nullptr) {
		MeetingWidget = CreateWidget<UMeetingDepWidget>(UGameplayStatics::GetPlayerController(this,0), UserWidget);
	}

	TakeIdea();
}

void AMeetingDepartment::TakeIdea()
{
	//if (MeetingWidget != nullptr && UserWidget != nullptr) {
	//	MeetingWidget->T_Genre->SetText(FText::FromString("TEST2"));
	//	GEngine->AddOnScreenDebugMessage(105, 5.f, FColor::Red, "work");
	//}
	//else if (MeetingWidget == nullptr)
	//{
	//	GEngine->AddOnScreenDebugMessage(105, 5.f, FColor::Red, "NULPTR meetingwidget");
	//}
	//else if (UserWidget == nullptr) {
	//	GEngine->AddOnScreenDebugMessage(105, 5.f, FColor::Red, "NULPTR userwidget");
	//}
	//MeetingWidget->AddToViewport();
}

void AMeetingDepartment::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (Cast<AProjectIdleCharacter>(OtherActor) != nullptr)
	{
		MeetingWidget->AddToViewport();
	}
}
void AMeetingDepartment::NotifyActorEndOverlap(AActor* OtherActor)
{
	if (Cast<AProjectIdleCharacter>(OtherActor) != nullptr)
	{
		MeetingWidget->RemoveFromParent();
	}
}

// Called every frame
void AMeetingDepartment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AMeetingDepartment::MoveToMeeting()
{
	int32 chairSize = GM->MeetingChairList.Num();
	int32 employeeSize = GM->EmployeeList.Num();
	int32 LoopUntil;

	FString sizeString = FString::FromInt(chairSize);
	UE_LOG(LogActor, Warning, TEXT("%s"), *sizeString)


		bool MoreEmployeeThanChair = false;

	if (chairSize > employeeSize)
	{
		LoopUntil = chairSize - employeeSize;
	}
	else if (employeeSize > chairSize)
	{
		//If there too many employee
		MoreEmployeeThanChair = true;
		LoopUntil = employeeSize;
	}
	else
	{
		LoopUntil = chairSize;
	}

	for (int i = 0; i < LoopUntil; i++)
	{
		if (MoreEmployeeThanChair)
		{
			if (i < chairSize)
			{
				GM->EmployeeList[i]->ToMeeting(GM->MeetingChairList[i]->GetActorLocation());
			}
			else
			{
				GM->EmployeeList[i]->ToMeeting(OfficeLocation);
			}
		}
		else
		{
			GM->EmployeeList[i]->ToMeeting(GM->MeetingChairList[i]->GetActorLocation());
		}
	}
}

void AMeetingDepartment::BackFromMeeting()
{
	int32 employeeSize = GM->EmployeeList.Num();

	for (int i = 0; i < employeeSize; i++)
	{
		GM->EmployeeList[i]->ReturnPositionAfterMeeting(GM->EmployeeList[i]->StartPosition);
	}

}