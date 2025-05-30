// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/KnifeSkeletonEnemy.h"

AKnifeSkeletonEnemy::AKnifeSkeletonEnemy()
{
	PawnTag = PawnTags::KnifeSkeleton;
}

void AKnifeSkeletonEnemy::TemporarilyHideWeapon(float HideDuration)
{
	if (!IsValid(WeaponMesh)) return;

	// 무기 숨기기
	WeaponMesh->SetVisibility(false, true); // bPropagateToChildren = true

	// 일정 시간 후 다시 보이게 설정
	GetWorldTimerManager().SetTimer(
		HideWeaponTimerHandle,
		this,
		&AKnifeSkeletonEnemy::ShowWeapon,
		HideDuration,
		false
	);
}

void AKnifeSkeletonEnemy::ShowWeapon()
{
	if (!IsValid(WeaponMesh)) return;

	WeaponMesh->SetVisibility(true, true);
}
