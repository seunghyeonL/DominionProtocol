// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss3Enemy.h"
#include "Engine/StaticMeshActor.h"

void ABoss3Enemy::Attack()
{
	if (AttackMontage == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage))
	{
		AnimInstance->Montage_Play(AttackMontage);
	}
}

void ABoss3Enemy::Attack2()
{
	if (AttackMontage2 == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage2))
	{
		AnimInstance->Montage_Play(AttackMontage2);
	}
}

void ABoss3Enemy::Attack3()
{
	if (AttackMontage3 == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage3))
	{
		AnimInstance->Montage_Play(AttackMontage3);
	}

	// 이전 돌 제거
	if (SpawnedStoneMeshComp)
	{
		SpawnedStoneMeshComp->DestroyComponent();
		SpawnedStoneMeshComp = nullptr;
	}

	// ⏱️ 0.5초 후에 SpawnStone() 실행
	GetWorld()->GetTimerManager().SetTimer(
		StoneSpawnDelayHandle,
		this,
		&ABoss3Enemy::SpawnStone,
		0.5f,
		false
	);
}


void ABoss3Enemy::SpawnStone()
{
	if (!StoneMesh) return;

	// 돌 생성
	SpawnedStoneMeshComp = NewObject<UStaticMeshComponent>(this);
	if (SpawnedStoneMeshComp)
	{
		SpawnedStoneMeshComp->RegisterComponent();
		SpawnedStoneMeshComp->SetStaticMesh(StoneMesh);
		SpawnedStoneMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// ThrowSocket에 부착
		SpawnedStoneMeshComp->AttachToComponent(
			GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale,
			TEXT("ThrowSocket")
		);

		// 3초 후 제거
		GetWorld()->GetTimerManager().SetTimer(
			StoneDestroyHandle,
			[this]()
			{
				if (IsValid(SpawnedStoneMeshComp))
				{
					SpawnedStoneMeshComp->DestroyComponent();
					SpawnedStoneMeshComp = nullptr;
				}
			},
			1.0f,
			false
		);
	}
}

void ABoss3Enemy::Attack4()
{
	if (AttackMontage4 == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(AttackMontage4))
	{
		AnimInstance->Montage_Play(AttackMontage4);
	}
}