#pragma once

#include "CoreMinimal.h"
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

	virtual void StartTrace();
	virtual void StopTrace();
	
	UFUNCTION(BlueprintCallable)
	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime);

	virtual float GetStamina() const;

	virtual void Tick(float DeltaTime);

	TObjectPtr<UAnimMontage> GetAnimMontage() const;

	FORCEINLINE FGameplayTag GetSkillTag() const { return SkillTag; }
	FORCEINLINE FGameplayTag GetControlEffectTag() const { return ControlEffectTag; }

protected:
	float GetFinalAttackData(const float AttackPower) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TObjectPtr<USoundBase> Sound = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TObjectPtr<UParticleSystem> Particle = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
	TArray<FEffectData> Effects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTag SkillTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag")
	FGameplayTag ControlEffectTag;

	float Stamina;

	float AttackRadius;

	float AttackForwardOffset;

	float DamageCoefficient;
};
