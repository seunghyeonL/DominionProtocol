#pragma once

#include "CoreMinimal.h"
#include "Util/GameTagList.h"
#include "Util/BattleDataTypes.h"
#include "SkillData.generated.h"

class UNiagaraSystem;
class ACurvedProjectile;

USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	FName SkillName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TArray<TObjectPtr<USoundBase>> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TArray<TObjectPtr<UParticleSystem>> Particle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	TArray<TObjectPtr<UNiagaraSystem>> NaiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TArray<TObjectPtr<UMaterialInterface>> SkillMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TSubclassOf<ACurvedProjectile> CurvedProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	TArray<FEffectData> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float AttackForwardOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float DamageCoefficient;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Data")
	FGameplayTag SkillTag;
};
