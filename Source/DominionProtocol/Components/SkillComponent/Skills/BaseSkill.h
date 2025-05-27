#pragma once

#include "CoreMinimal.h"
#include "Util/BattleDataTypes.h"
#include "BaseSkill.generated.h"

class ACurvedProjectile;

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
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const;

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
	TArray<TObjectPtr<USoundBase>> Sound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TArray<TObjectPtr<UParticleSystem>> Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Data")
	TSubclassOf<ACurvedProjectile> CurvedProjectileClass;

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

	float AnimPlayRate;
};
