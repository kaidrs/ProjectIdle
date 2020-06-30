// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryButton.h"
#include "ProjectIdle/OfficeDepartment.h"
#include "ProjectIdle/CeoDepMenuWidget.h"
#include "ProjectIdle/Shop/Item.h"
#include "ProjectIdle/Workstation.h"
#include "ProjectIdle/GameManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

UInventoryButton::UInventoryButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UInventoryButton::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (ItemBP != nullptr)
	{
		Item = ItemBP.GetDefaultObject();
		Item_I->SetBrushFromTexture(Item->ItemImage);
		ItemName_T->SetText(FText::FromString(Item->ItemName));
	}
}

void UInventoryButton::NativeConstruct()
{
	Super::NativeConstruct();
	GameManager = GetWorld()->GetGameInstance<UGameManager>();

	if (!Item_Btn->OnClicked.IsBound())
	{
		Item_Btn->OnClicked.AddDynamic(this, &UInventoryButton::OnClicked);
	}
}

void UInventoryButton::OnClicked()
{
	switch (Item->ItemCategory)
	{
	case ECategory::ComputerComponents:
	case ECategory::DeskAndChairs:
		CurrentStation->UpgradeMesh(Item);
		break;
	case ECategory::Materials:
		if (Item->ItemSubCategory == ESubCategory::FloorMat)
		{
			GameManager->OfficeDepartment->OfficeDepMenuWidget->CurrentFloorMat_I->SetBrushFromTexture(Item->ItemImage);

			for (int i = 0; i < GameManager->FloorList.Num(); i++)
			{
				GameManager->FloorList[i]->UpdateWallMaterial(Item->Material->GetMaterial());
			}
		}
		else if (Item->ItemSubCategory == ESubCategory::WallMat)
		{
			GameManager->OfficeDepartment->OfficeDepMenuWidget->CurrentWallMat_I->SetBrushFromTexture(Item->ItemImage);
		}
		break;
	}
}