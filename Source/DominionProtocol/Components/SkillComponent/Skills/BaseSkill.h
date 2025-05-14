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

	virtual void Initialize(ACharacter* OwnerCharacter);

	virtual void Execute();

	virtual void AttackTrace() const;

	virtual float GetStamina() const;

	virtual void Tick(float DeltaTime);

	TObjectPtr<UAnimMontage> GetAnimMontage() const;

	FORCEINLINE FGameplayTag GetSkillTag() const { return SkillTag; }
	FORCEINLINE FGameplayTag GetControlEffectTag() const { return ControlEffectTag; }

protected:
	bool bIsExecuting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner", meta=(AllowPrivateAccess=true))
	ACharacter* OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag", meta=(AllowPrivateAccess=true))
	FGameplayTag SkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag", meta=(AllowPrivateAccess=true))
	FGameplayTag ControlEffectTag;
	
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
