// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss3Enemy.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Player/Characters/DomiCharacter.h"

ABoss3Enemy::ABoss3Enemy()
{
	PawnTag = PawnTags::Boss3;
}

void ABoss3Enemy::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

	SpawnDropItem();
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

	if (SpawnedStoneMeshComp)
	{
		SpawnedStoneMeshComp->DestroyComponent();
		SpawnedStoneMeshComp = nullptr;
	}

	GetWorld()->GetTimerManager().SetTimer(
		StoneSpawnDelayHandle,
		this,
		&ABoss3Enemy::SpawnStone,
		0.5f,
		false
	);
}

void ABoss3Enemy::SpawnStoneReady()
{
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
			1.4f,
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

void ABoss3Enemy::ThrowCapturedTarget()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
	if (!IsValid(CapturedTarget)) return;

	// 부착 해제
	CapturedTarget->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (UCharacterMovementComponent* MoveComp = CapturedTarget->GetCharacterMovement())
	{
		MoveComp->GravityScale = 1.0f;
	}
	if (UCapsuleComponent* Capsule = CapturedTarget->GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	if (USkeletalMeshComponent* TargetMesh = CapturedTarget->GetMesh())
	{
		TargetMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TargetMesh->SetSimulatePhysics(false);
	}

	// 던지는 방향: 골렘 앞쪽 + 위로
	FVector LaunchDirection = GetActorForwardVector() + FVector(0.f, 0.f, 0.2f);
	LaunchDirection.Normalize();

	CapturedTarget->LaunchCharacter(LaunchDirection * 2500.f, true, true);

	// 상태 초기화
	CapturedTarget = nullptr;
}

void ABoss3Enemy::OnGroggy()
{
	Super::OnGroggy();
	StatusComponent->ActivateStatusEffect(EffectTags::Groggy, 0.f, 3.f);
}

void ABoss3Enemy::SpawnDropItem()
{
	// 일반 DroppedItem 로직
	if (IsValid(DropItemClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor<AItemDropped>(DropItemClass, GetGroundSpawnLocation(), GetActorRotation(), SpawnParams);
	}

	// 스토리 필수 아이템 자동 습득
	if (StoryItemTag.IsValid())
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
		if (IsValid(PlayerCharacter))
		{
			UItemComponent* ItemComponent = PlayerCharacter->FindComponentByClass<UItemComponent>();
			if (IsValid(ItemComponent))
			{
				ItemComponent->AddItem(StoryItemTag, 1);
			}
		}
	}
}
