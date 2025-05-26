#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"
#include "RailGunSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API URailGunSkill : public UCurvedProjectileSkill
{
	GENERATED_BODY()
	
public:
	URailGunSkill();

	virtual void Execute() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) override;
};
