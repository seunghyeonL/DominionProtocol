#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"
#include "FireBallSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UFireBallSkill : public UCurvedProjectileSkill
{
	GENERATED_BODY()
	
public:
	UFireBallSkill();
 
	virtual void Execute() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const override;
};
