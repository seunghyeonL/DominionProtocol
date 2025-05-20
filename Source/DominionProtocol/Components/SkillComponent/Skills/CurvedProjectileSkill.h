#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "CurvedProjectileSkill.generated.h"

class UObjectPoolSubsystem;

UCLASS()
class DOMINIONPROTOCOL_API UCurvedProjectileSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UCurvedProjectileSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	int32 TotalProjectileCount = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LaunchInterval = 0.3f;

private:
	void LaunchNextProjectile();

	FORCEINLINE void SetTargetActor(AActor* NewActor) { TargetActor = NewActor; }

	void SetTargetActorInPublicSpace();

	bool IsActorInViewport(const FVector& ActorLocation) const;

	UObjectPoolSubsystem* ObjectPoolSubsystem;

	AActor* TargetActor;

	FTimerHandle FireTimerHandle;

	int32 ProjectileIndexToLaunch = 0;

	FVector SpawnLocation;
	FRotator SpawnRotator;

	FVector TargetLocation;
};
