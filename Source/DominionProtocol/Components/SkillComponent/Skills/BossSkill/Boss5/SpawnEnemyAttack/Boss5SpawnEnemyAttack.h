#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss5SpawnEnemyAttack.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UBoss5SpawnEnemyAttack : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UBoss5SpawnEnemyAttack();

	virtual void Execute() override;

};