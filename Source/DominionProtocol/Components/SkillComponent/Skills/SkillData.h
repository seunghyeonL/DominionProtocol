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

	FSkillData()
		: bIsMagicSkill(false),
		  CoolDownTime(0.f),
		  AnimMontage(nullptr),
		  Sounds(TArray<TObjectPtr<USoundBase>>()),
		  Particles(TArray<TObjectPtr<UParticleSystem>>()),
		  NiagaraParticles(TArray<TObjectPtr<UNiagaraSystem>>()),
		  SkillMaterials(TArray<TObjectPtr<UMaterialInterface>>()),
		  CurvedProjectileClass(nullptr),
		  Effects(TArray<FEffectData>()),
		  Stamina(0.f),
		  AttackRadius(0.f),
		  AttackForwardOffset(0.f),
		  DamageCoefficient(0.f),
		  GroggyDamage(0.f),
		  LaunchGroundSpeed(0.f),
		  LaunchZSpeed(0.f),
		  SkillTag(FGameplayTag())
	{
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	FName SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	bool bIsMagicSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float CoolDownTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TArray<TObjectPtr<UParticleSystem>> Particles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill Data")
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraParticles;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float GroggyDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float LaunchGroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	float LaunchZSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Skill Data")
	FGameplayTag SkillTag;
};
