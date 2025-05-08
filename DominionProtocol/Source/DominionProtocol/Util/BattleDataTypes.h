#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
//#include "Components/Skill.h"
#include "BattleDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FEffectData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Magnitude { 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration { 0.f };
};

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* Instigator { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage { 0.f };
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LaunchVector { 0.f, 0.f, 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEffectData> Effects;
};

USTRUCT(BlueprintType)
struct FSkillGroup
{
	GENERATED_BODY()

	// 스킬 그룹 내 스킬 목록
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TArray<TSubclassOf<USkill>> Skills;

	int32 CurrentSkillIdx;
};