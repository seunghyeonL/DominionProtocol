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
	void Initialize();

	void Execute(ACharacter* Owner);

	void AttackTrace(ACharacter* Owner) const;

	float GetStamina() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTag SkillTag;

private:
	float GetFinalAttackData(const float AttackPower) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset", meta=(AllowPrivateAccess=true))
	TObjectPtr<USoundBase> Sound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset", meta=(AllowPrivateAccess=true))
	TObjectPtr<UParticleSystem> Particle = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = true))
	TArray<FEffectData> Effects;

	float Stamina;

	float AttackRadius;

	float AttackForwardOffset;

	float DamageCoefficient;
};
