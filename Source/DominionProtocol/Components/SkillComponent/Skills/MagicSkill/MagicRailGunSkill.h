#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/MagicSkill/MagicCurvedProjectileSkill.h"
#include "MagicRailGunSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UMagicRailGunSkill : public UMagicCurvedProjectileSkill
{
	GENERATED_BODY()
	
public:
	UMagicRailGunSkill();

	virtual void Execute() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const override;
};
