// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/GunMinionEnemy.h"

AGunMinionEnemy::AGunMinionEnemy()
{
	PawnTag = PawnTags::GunMinion;

	EssenceAmount = 55.f;
}

void AGunMinionEnemy::Attack()
{
	if (AttackMontage == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Play(AttackMontage);
	}
}