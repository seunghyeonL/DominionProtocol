#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Util/GameTagList.h"
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

	//장비아이템 중복획득 방지
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	int32 MaxItemQuantity;
	
	//아이템태그(Util/GameTagList, Config/DefaultGameplayTags.ini, DT_ItemDataTable 행이름 변환해 사용) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FGameplayTag ItemTag;

	// Weapon Skill Data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FSkillComponentInitializeData WeaponSkillData;

	// 아이템 클래스 (액터)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSubclassOf<class ABaseItem> ItemClass;

	//무기 메시
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	UStaticMesh* WeaponMesh;

};

// Item.Consume.이름
// Item.Equip.Weapon.이름
// Item.Misc.이름

// 무기?
// SkillData