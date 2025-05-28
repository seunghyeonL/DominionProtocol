#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "CurvedProjectileSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UCurvedProjectileSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const override;

	UPROPERTY()
	TObjectPtr<AActor> TargetActor;
	virtual bool CheckParry(AActor* HitActor) const override;

private:
	void UpdateTarget();

	void SetPlayerAsTarget();
};
