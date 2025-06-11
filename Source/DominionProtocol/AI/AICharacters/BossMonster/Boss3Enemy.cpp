// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss3Enemy.h"

ABoss3Enemy::ABoss3Enemy()
{

}

void ABoss3Enemy::Attack()
{
	if (AttackMontage == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Play(AttackMontage);
	}
}