#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/MagicSkill/MagicCurvedProjectileSkill.h"
#include "MagicFireBallSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UMagicFireBallSkill : public UMagicCurvedProjectileSkill
{
	GENERATED_BODY()
	
public:
	UMagicFireBallSkill();
 
	virtual void Execute() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const override;
};
