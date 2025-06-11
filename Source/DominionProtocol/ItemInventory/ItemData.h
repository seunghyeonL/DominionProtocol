#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "ItemInventory/WeaponData.h"
#include "ItemInventory/AccessoryData.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "Util/GameTagList.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Equippable	UMETA(DisplayName = "Equippable"),
	Consumable	UMETA(DisplayName = "Consumable"),
	Other		UMETA(DisplayName = "Other")

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
	FSkillComponentInitializeData SkillDatas;

	// 아이템 클래스 (액터)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	TSubclassOf<class ABaseItem> ItemClass;

	//드랍 전용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Data")
	UStaticMesh* ItemStaticMesh;

	//무기 메시 소켓 등의 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FWeaponData WeaponData;

	//무기 계수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	float WeaponStatMultiplier;

	//악세서리 메시 소켓 등의 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FAccessoryData AccessoryData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Effect")
	class UAnimMontage* ConsumeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consumable Effect")
	class UNiagaraSystem* ConsumeNiagaraSystem;

	//(UI)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	TMap<FGameplayTag,float> StatChangeMap;

	//(UI)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	FText ConsumableEffectText;

};
