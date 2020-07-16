// Fill out your copyright notice in the Description page of Project Settings.


#include "IdeaBacklogWidget.h"
#include "Engine.h"
#include "ProjectIdle/Idea.h"
#include "ProjectIdle/GameManager.h"
#include "ProjectIdle/MeetingDepartment.h"
#include "ProjectIdle/OfficeDepartment.h"
#include "ProjectIdle/Employees/FloorManager.h"
#include "MeetingDepWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "IdeaButton.h"

void UIdeaBacklogWidget::NativeConstruct() {
	Super::NativeConstruct();

	if (!Back_Btn->OnClicked.IsBound())
	{
		Back_Btn->OnClicked.AddDynamic(this, &UIdeaBacklogWidget::Return);
	}
	if (!CallMeetingBtn->OnClicked.IsBound())
	{
		CallMeetingBtn->OnClicked.AddDynamic(this, &UIdeaBacklogWidget::CallMeeting);
	}
}

void UIdeaBacklogWidget::DisplayNewIdea(Idea* idea) {

	GEngine->AddOnScreenDebugMessage(5, 5.f, FColor::Red, "Displaying new idea");

	idea->IdeaButton = Cast<UIdeaButton>(CreateWidget(this, IdeaButtonWidgetClass));
	AddValuesToButton(idea);

	IdeaScrollBox->AddChild(idea->IdeaButton);
}

void UIdeaBacklogWidget::Return()
{
	OfficeDepartment->BacklogReturn();
	//GM->OfficeDepartmentList[GM->OfficeDepartment->FloorLevel - 1]->BacklogReturn();
	CallMeetingBtn->SetIsEnabled(false);
}

void UIdeaBacklogWidget::AddValuesToButton(Idea* idea)
{
	idea->IdeaButton->GameCover_I->SetColorAndOpacity(idea->CoverColor);

	idea->IdeaButton->GameTitle_T->SetText(FText::FromString(idea->IdeaName));
	idea->IdeaButton->GameDescription_T->SetText(FText::FromString(idea->IdeaDescription));
	idea->IdeaButton->Genre_T->SetText(Idea::GenreToText(idea->Genre));
	idea->IdeaButton->SuccessChance_T->SetText(FText::AsPercent(idea->SuccessChance / 100.f));

	if (idea->ProgrammerWorkload > (idea->ArtistWorkload + 20)) {
		idea->IdeaButton->Weight_T->SetText(FText::FromString("Programmer"));
	}
	else if (idea->ArtistWorkload > (idea->ProgrammerWorkload + 20)) {
		idea->IdeaButton->Weight_T->SetText(FText::FromString("Artist"));
	}
	else
	{
		idea->IdeaButton->Weight_T->SetText(FText::FromString("All"));
	}

	idea->IdeaButton->BacklogWidget = this;
	idea->IdeaButton->OfficeDepartment = OfficeDepartment;

	idea->IdeaButton->storedIndex = OfficeDepartment->Index;
}

void UIdeaBacklogWidget::CallMeeting()
{
	if (GM == nullptr)
	{
		GM = GetWorld()->GetGameInstance<UGameManager>();
		GEngine->AddOnScreenDebugMessage(101, 5.f, FColor::Red, "populate GM");
	}
	//if (GM->Character->CurrentFloor == 1)
	//{
		//int CurrentFloorLevel = 0;

		//if (GM->OfficeDepartment->ManagerRef->AutoManaging)
		//{
		//	CurrentFloorLevel = GM->OfficeDepartment->ManagerRef->FloorLevel;
		//}
		//else
		//{
		//	CurrentFloorLevel = GM->Character->CurrentFloor;
		//}
		//GM->OfficeDepartmentList[CurrentFloorLevel]->IdeaList[ChosenIndex]->IdeaButton->IdeaButton->SetIsEnabled(false);
		OfficeDepartment->IdeaList[ChosenIndex]->IdeaButton->IdeaButton->SetIsEnabled(false);
		//UIdeaButton::IsInProduction = true;
		GM->IdeaInProduction = true;
		CallMeetingBtn->SetIsEnabled(false);
		GM->MeetingDepartment->MoveToMeeting();
		//GM->MeetingDepartmentList[OfficeDepartment->FloorLevel - 1]->MoveToMeeting();
		SendIdea(ChosenIndex);
	//}
}
void UIdeaBacklogWidget::CallMeeting_M(AFloorManager* ManagerRef)
{
	if (GM == nullptr)
	{
		GM = GetWorld()->GetGameInstance<UGameManager>();
	}
	//GM->OfficeDepartmentList[GM->OfficeDepartment->FloorLevel - 1]->IdeaList[ManagerRef->IdeaIndex]->IdeaButton->IdeaButton->SetIsEnabled(false);
	OfficeDepartment->IdeaList[ManagerRef->IdeaIndex]->IdeaButton->IdeaButton->SetIsEnabled(false);
	//UIdeaButton::IsInProduction = true;
	GM->IdeaInProduction = true;
	CallMeetingBtn->SetIsEnabled(false);
	//GM->MeetingDepartmentList[GM->OfficeDepartment->FloorLevel - 1]->MoveToMeeting();
	GM->MeetingDepartment->MoveToMeeting();
	ManagerRef->MeetingState = true;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Movetomeeting");

	//ManagerRef->MoveEmployee();
	SendIdea(ManagerRef->IdeaIndex);
}
//Called by CallMeeting
void UIdeaBacklogWidget::SendIdea(int Index) {

	//Gm populated in CallMeeting
	if (GM->MeetingDepartment == nullptr) {
		GEngine->AddOnScreenDebugMessage(102, 5.f, FColor::Red, "GM->MeetingDepartment is null");
	}
	else if (GM->MeetingDepartment != nullptr) {
		auto tIdea = OfficeDepartment->IdeaList[Index];
		//auto tIdea = GM->OfficeDepartmentList[GM->OfficeDepartment->FloorLevel - 1]->IdeaList[Index];
		if (tIdea != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(1036, 5.f, FColor::Red, "tIdea work");
			GM->MeetingDepartment->TakeIdea(tIdea);
			//GM->MeetingDepartmentList[0]->TakeIdea(tIdea);
		}
		//GM->MeetingDepartment->TakeIdea(OfficeDepartment->IdeaList[ChosenIndex]);
		GEngine->AddOnScreenDebugMessage(103, 5.f, FColor::Red, "Meeting TakeIdea");
	}
}
