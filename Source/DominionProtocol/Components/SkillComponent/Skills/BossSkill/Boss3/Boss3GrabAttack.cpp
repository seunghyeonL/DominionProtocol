// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/BossSkill/Boss3/Boss3GrabAttack.h"
#include "AI/AICharacters/BossMonster/Boss3Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"

UBoss3GrabAttack::UBoss3GrabAttack()
{
	SkillTag = SkillTags::Boss3GrabAttack;
}

void UBoss3GrabAttack::AttackTrace() const
{
	if (ABoss3Enemy* Boss = Cast<ABoss3Enemy>(OwnerCharacter))
	{
		Boss->ThrowCapturedTarget(); // 잡은 캐릭터 던짐
	}
}