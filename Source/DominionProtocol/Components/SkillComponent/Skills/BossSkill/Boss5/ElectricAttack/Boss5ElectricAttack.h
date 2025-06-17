#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss5ElectricAttack.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UBoss5ElectricAttack : public UBaseSkill
{
	GENERATED_BODY()
	
public: 
	UBoss5ElectricAttack();

	virtual void Execute()override;
};
