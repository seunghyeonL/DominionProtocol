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
	virtual void Initialize();

	virtual void Execute(ACharacter* Owner);

	virtual void AttackTrace(ACharacter* Owner) const;

	virtual float GetStamina() const;

	FORCEINLINE FGameplayTag GetSkillTag() const { return SkillTag; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTag SkillTag;
	
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
