#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "CurvedProjectileSkill.generated.h"

//class UObjectPoolSubsystem;

UCLASS()
class DOMINIONPROTOCOL_API UCurvedProjectileSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UCurvedProjectileSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	int32 TotalProjectileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LaunchInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	UBaseSkill* CurrentSkill;

private:
	void UpdateTarget();

	void ProjectileFromPool();

	void SetPlayerAsTarget();

	//UPROPERTY()
	//UObjectPoolSubsystem* ObjectPoolSubsystem;

	UPROPERTY()
	ACurvedProjectile* CurvedProjectile;

	UPROPERTY()
	AActor* TargetActor;

	FTimerHandle FireTimerHandle;

	int32 ProjectileIndexToLaunch = 0;
};
