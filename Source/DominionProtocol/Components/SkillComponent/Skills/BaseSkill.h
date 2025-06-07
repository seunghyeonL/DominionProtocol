#pragma once

#include "CoreMinimal.h"
#include "NiagaraCore.h"
#include "Util/BattleDataTypes.h"
#include "Util/GameTagList.h"
#include "BaseSkill.generated.h"

class UNiagaraSystem;
class ACurvedProjectile;

UCLASS(Blueprintable)
class DOMINIONPROTOCOL_API UBaseSkill : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseSkill();

	UFUNCTION(BlueprintCallable)
	virtual void Initialize(ACharacter* OwnerCharacter);

	virtual void Execute();

	virtual void AttackTrace() const;

	virtual void StartTrace(const FGameplayTagContainer& TagContainer);
	virtual void StopTrace(const FGameplayTagContainer& TagContainer);
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const;

	virtual float GetStamina() const;

	virtual void Tick(float DeltaTime);

	FORCEINLINE UAnimMontage* GetAnimMontage() const { return AnimMontage; }
	FORCEINLINE FGameplayTag GetSkillTag() const { return SkillTag; }
	FORCEINLINE FGameplayTag GetControlEffectTag() const { return ControlEffectTag; }
	FORCEINLINE TObjectPtr<ACharacter> GetOwnerCharacter() const { return OwnerCharacter; }
	FORCEINLINE TSubclassOf<ACurvedProjectile> GetCurvedProjectileClass() const { return CurvedProjectileClass; }
	FORCEINLINE const TArray<USoundBase*>& GetSounds() const { return Sound; }
	FORCEINLINE const TArray<UParticleSystem*>& GetParticles() const { return Particle; }
	FORCEINLINE const TArray<UNiagaraSystem*>& GetNiagaraParticles() const { return NiagaraParticle; }
	FORCEINLINE const TArray<FEffectData>& GetEffects() const { return Effects; }
	FORCEINLINE float GetDamageCoefficient() const { return DamageCoefficient; }
	float GetFinalAttackData(const float AttackPower) const;

protected:
	virtual bool CheckParry(AActor* HitActor) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
	TArray<TObjectPtr<USoundBase>> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
	TArray<TObjectPtr<UParticleSystem>> Particle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
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
