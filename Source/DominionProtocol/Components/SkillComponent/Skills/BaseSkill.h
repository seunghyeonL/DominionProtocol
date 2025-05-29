#pragma once

#include "CoreMinimal.h"
#include "NiagaraCore.h"
#include "Util/BattleDataTypes.h"
#include "Util/GameTagList.h"
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

	FORCEINLINE UAnimMontage* GetAnimMontage() const { return AnimMontage; }
	FORCEINLINE FGameplayTag GetSkillTag() const { return SkillTag; }
	FORCEINLINE FGameplayTag GetControlEffectTag() const { return ControlEffectTag; }
	FORCEINLINE TSubclassOf<ACurvedProjectile> GetCurvedProjectileClass() const { return CurvedProjectileClass; }
	FORCEINLINE const TArray<USoundBase*>& GetSounds() const { return Sound; }
	FORCEINLINE const TArray<UParticleSystem*>& GetParticles() const { return Particle; }
	FORCEINLINE const TArray<FEffectData>& GetEffects() const { return Effects; }
	FORCEINLINE float GetDamageCoefficient() const { return DamageCoefficient; }
	float GetFinalAttackData(const float AttackPower) const;

protected:
	
	virtual bool CheckParry(AActor* HitActor) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TArray<USoundBase*> Sound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TArray<UParticleSystem*> Particle;

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
