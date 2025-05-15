#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
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
};