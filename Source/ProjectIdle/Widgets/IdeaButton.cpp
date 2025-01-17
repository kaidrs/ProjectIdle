// Fill out your copyright notice in the Description page of Project Settings.


#include "IdeaButton.h"
#include "Engine.h"
#include "IdeaBacklogWidget.h"
#include "Components/Button.h"
#include "ProjectIdle/Idea.h"
#include "ProjectIdle/GameManager.h"
#include "ProjectIdle/OfficeDepartment.h"
#include "ProjectIdle/CeoDepMenuWidget.h"

UIdeaButton::UIdeaButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UIdeaButton::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IdeaButton->OnClicked.IsBound())
	{
		IdeaButton->OnClicked.AddDynamic(this, &UIdeaButton::ButtonClicked);
	}
}

void UIdeaButton::ButtonClicked()
{
	auto GM = GetWorld()->GetGameInstance<UGameManager>();
	if (IsFinished && !IsPublished)
	{
		OfficeDepartment->OfficeDepMenuWidget->PublishGame_Btn->SetIsEnabled(true);
		OfficeDepartment->OfficeDepMenuWidget->ChosenIndex = storedIndex;
		OfficeDepartment->OfficeDepMenuWidget->IdeaGeneratedMoney_T->SetText(FText::FromString("Not yet Published."));
		IdeaButton->SetBackgroundColor(PublishedColor);
	}
	else if (IsPublished)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, "The game is published");
		OfficeDepartment->OfficeDepMenuWidget->IdeaGeneratedMoney_T->SetText(FText::AsCurrency(MoneyGenerated));
		OfficeDepartment->OfficeDepMenuWidget->PublishGame_Btn->SetIsEnabled(false);
		DisplayStatistics();
	}
	//else if (!GM->IdeaInProduction)
	else if(!GM->OfficeDepartmentList[GM->Character->CurrentFloor - 1]->IdeaCurrentFloor)
	{
		BacklogWidget->ChosenIndex = storedIndex;
		//BacklogWidget->CallMeetingBtn->SetIsEnabled(true);
		GM->OfficeDepartmentList[GM->Character->CurrentFloor - 1]->BacklogWidget->CallMeetingBtn->SetIsEnabled(true);

	}
}

void UIdeaButton::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IdeaButton)
	{
		if (IdeaButton->HasUserFocus(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			IdeaButton->SetBackgroundColor(SelectedColor);
		}
		else
		{
			if (IsPublished)
			{
				IdeaButton->SetBackgroundColor(PublishedColor);
			}
			else
			{
				IdeaButton->SetBackgroundColor(FLinearColor::White);
			}
		}
	}
}

void UIdeaButton::DisplayStatistics()
{
	FString stadisticText = "\t\t" + IdeaReference->IdeaName + "\n" + "Downloads: " + FString::FromInt(Downloads) + "\nTrend: " + (IsSuccessful ? "Successful" : "Unsuccessful");

	OfficeDepartment->OfficeDepMenuWidget->IdeaStadistics_T->SetText(FText::FromString(stadisticText));
}
