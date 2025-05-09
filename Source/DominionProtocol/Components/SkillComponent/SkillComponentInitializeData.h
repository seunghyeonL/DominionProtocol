#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "ItemInventory/BaseItem.h"
#include "SkillComponentInitializeData.generated.h"

USTRUCT(BlueprintType)
struct FSkillGroupInitializeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SkillGroupTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UBaseSkill>> SkillGroupData;
};

USTRUCT(BlueprintType)
struct FSkillComponentInitializeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSkillGroupInitializeData> SkillGroupInitializeDatas;
	
	// SkillTags::BaseAttack
	// "Skill.BaseAttack", { UDomiCharacterBaseAttackCombo1::StaticClass(),UDomiCharacterBaseAttackCombo2::StaticClass())
	//							Skill.BaseAttack.first							Skill.BaseAttack.Second

	// "Skill.BaseAttack", { UBaseAttack::StaticClass() }
	//							Skill.BaseAttack.first
};