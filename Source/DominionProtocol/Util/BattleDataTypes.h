#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "BattleDataTypes.generated.h"

class UBaseSkill;

USTRUCT(BlueprintType)
struct FSkillGroup
{
	GENERATED_BODY()

	// Skills[i] == i번째 콤보 단계의 스킬 (0번째가 첫 번째 콤보, 1번째가 두 번째 콤보 ...)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<UBaseSkill*> Skills;

	// Skills 배열에서 현재 진행 중인 콤보 공격의 인덱스
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ComboIdx { 0 };
};


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