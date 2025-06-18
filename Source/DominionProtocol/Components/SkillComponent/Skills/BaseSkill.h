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
	virtual void AttackTrace_Cylinder(FVector Offset, float Radius, float HalfHeight) const;
	virtual void StartTrace(const FGameplayTagContainer& TagContainer);
	virtual void StopTrace(const FGameplayTagContainer& TagContainer);
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const;

	virtual float GetStamina() const;

	virtual void Tick(float DeltaTime);

	void CoolDownStart();

	FORCEINLINE UAnimMontage* GetAnimMontage() const { return AnimMontage; }
	FORCEINLINE FGameplayTag GetSkillTag() const { return SkillTag; }
	FORCEINLINE FGameplayTag GetControlEffectTag() const { return ControlEffectTag; }
	FORCEINLINE TObjectPtr<ACharacter> GetOwnerCharacter() const { return OwnerCharacter; }
	FORCEINLINE TSubclassOf<ACurvedProjectile> GetCurvedProjectileClass() const { return CurvedProjectileClass; }
	FORCEINLINE const TArray<USoundBase*>& GetSounds() const { return Sounds; }
	FORCEINLINE const TArray<UParticleSystem*>& GetParticles() const { return Particles; }
	FORCEINLINE const TArray<UNiagaraSystem*>& GetNiagaraParticles() const { return NiagaraParticles; }
	FORCEINLINE const TArray<FEffectData>& GetEffects() const { return Effects; }
	FORCEINLINE float GetDamageCoefficient() const { return DamageCoefficient; }
	float GetFinalAttackData() const;
	FORCEINLINE float GetLaunchGroundSpeed() const { return LaunchGroundSpeed; }
	FORCEINLINE float GetLaunchZSpeed() const { return LaunchZSpeed; }
	FORCEINLINE float IsMagicSkill() const { return bIsMagicSkill; }
	FORCEINLINE float IsCoolDowning() const { return bIsCoolDowning; }
	FORCEINLINE float GetGroggyDamage() const { return GroggyDamage; }

protected:
	virtual bool CheckParry(AActor* HitActor) const;

	bool bIsMagicSkill;
	bool bIsCoolDowning;
	
	FTimerHandle CoolDownTimer;

	void CoolDownEnd();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner")
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Asset")
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asset")
	TArray<TObjectPtr<UParticleSystem>> Particles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Asset")
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraParticles;

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

	float LaunchGroundSpeed;

	float LaunchZSpeed;

	float CoolDownTime;

	float GroggyDamage;
};
