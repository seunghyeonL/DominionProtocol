#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"
#include "FireballSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UFireballSkill : public UCurvedProjectileSkill
{
	GENERATED_BODY()
	
public:
	UFireballSkill();
 
	virtual void Execute() override;

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) override;

};
