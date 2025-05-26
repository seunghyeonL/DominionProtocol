#pragma once

#include "CoreMinimal.h"
#include "Util/GameTagList.h"
#include "ItemData.h"
#include "ItemUISlotData.generated.h"

USTRUCT(BlueprintType)
struct FItemUISlotData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Slot Data")
	FGameplayTag ItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Slot Data")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Slot Data")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Slot Data")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Slot Data")
	int32 CurrentQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Slot Data")
	int32 MaxStackQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI Slot Data")
	EItemType ItemType;

	FItemUISlotData()
		: ItemTag(FGameplayTag())
		, ItemIcon(nullptr)
		, ItemName(FString())
		, ItemDescription(FText::GetEmpty())
		, CurrentQuantity(0)
		, MaxStackQuantity(1)
		, ItemType(EItemType::Consumable)
	{
	}
};