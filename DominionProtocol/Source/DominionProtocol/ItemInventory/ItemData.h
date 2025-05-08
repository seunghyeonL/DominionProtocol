#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Consumable UMETA(DisplayName = "Consumable")

};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Item Data")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 MaxItemQuantity;
	
	//이런식으로 쓰는건가요?
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FName ItemTag;
};
