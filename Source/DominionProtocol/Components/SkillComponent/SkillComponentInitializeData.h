#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "ItemInventory/BaseItem.h"
#include "SkillComponentInitializeData.generated.h"

USTRUCT(BlueprintType)
struct FSkillGroup
{
	GENERATED_BODY()

	// 스킬 그룹 내 스킬 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UBaseSkill>> SkillGroup;

	int32 CurrentSkillIdx;
};

USTRUCT(BlueprintType)
struct FSkillComponentInitializeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, FSkillGroup> SkillGroups;
	// SkillTags::BaseAttack
	// "Skill.BaseAttack", { UDomiCharacterBaseAttackCombo1::StaticClass(),UDomiCharacterBaseAttackCombo2::StaticClass())
	//							Skill.BaseAttack.first							Skill.BaseAttack.Second

	// "Skill.BaseAttack", { UBaseAttack::StaticClass() }
	//							Skill.BaseAttack.first
};