// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/BossSkill/Boss3/Boss3RockThrow.h"
#include "AI/AICharacters/BossMonster/Boss3Enemy.h"

UBoss3RockThrow::UBoss3RockThrow()
{
	SkillTag = SkillTags::Boss3RockThrow;
}

void UBoss3RockThrow::Execute()
{
	Super::Execute();

	if (!IsValid(OwnerCharacter)) return;

	if (ABoss3Enemy* Boss = Cast<ABoss3Enemy>(OwnerCharacter))
	{
		Boss->SpawnStone();
	}
}