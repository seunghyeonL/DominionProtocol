#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "MagicCurvedProjectileSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UMagicCurvedProjectileSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const override;

	virtual bool CheckParry(AActor* HitActor) const override;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;

private:
	void UpdateTarget();

	void SetPlayerAsTarget();
};
