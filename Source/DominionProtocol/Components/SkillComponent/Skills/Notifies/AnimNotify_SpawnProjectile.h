#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_SpawnProjectile.generated.h"

class UCurvedProjectileSkill;

UCLASS()
class DOMINIONPROTOCOL_API UAnimNotify_SpawnProjectile : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	int32 TotalProjectileCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LaunchInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TArray<FVector> Offsets;

private:
	void ProjectileFromPool();

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> CachedMeshComp;

	UPROPERTY()
	TObjectPtr<AActor> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

	UPROPERTY()
	TObjectPtr<UCurvedProjectileSkill> ProjectileSkill;

	FGameplayTag SkillTag;

	int32 ProjectileIndexToLaunch;

	float ElapsedTime;
};
