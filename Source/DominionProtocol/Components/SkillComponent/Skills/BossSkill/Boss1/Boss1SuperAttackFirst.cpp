// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/BossSkill/Boss1/Boss1SuperAttackFirst.h"
#include "Util/GameTagList.h"

UBoss1SuperAttackFirst::UBoss1SuperAttackFirst()
{
	SkillTag = SkillTags::Boss1SuperAttack1;
}

void UBoss1SuperAttackFirst::Execute()
{
	AnimPlayRate = FMath::RandRange(0.5f, 0.8f);

	Super::Execute();
}
