#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Util/BattleDataTypes.h"
#include "BaseSkill.generated.h"

UCLASS(Blueprintable)
class DOMINIONPROTOCOL_API UBaseSkill : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseSkill();

	void Initialize();

	void Execute(ACharacter* Owner);

	void AttackTrace(ACharacter* Owner) const;

	float GetStamina() const;

private:
	float GetFinalAttackData(const float AttackPower) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset", meta=(AllowPrivateAccess=true))
	TObjectPtr<USoundBase> Sound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset", meta=(AllowPrivateAccess=true))
	TObjectPtr<UParticleSystem> Particle = nullptr;

	float Stamina;

	float AttackRadius;

	float AttackForwardOffset;

	float DamageCoefficient;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag", meta = (AllowPrivateAccess = "true"))
	FGameplayTag SkillTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AttackData", meta=(AllowPrivateAccess=true))
	FAttackData BaseAttackData;
};
