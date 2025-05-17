#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "CurvedProjectileSkill.generated.h"

class CurvedProjectile;

UCLASS()
class DOMINIONPROTOCOL_API UCurvedProjectileSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UCurvedProjectileSkill();

	TObjectPtr<CurvedProjectile> CurvedProjectile;
};
