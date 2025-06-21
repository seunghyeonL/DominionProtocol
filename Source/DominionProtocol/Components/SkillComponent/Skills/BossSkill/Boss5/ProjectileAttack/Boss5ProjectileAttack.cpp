#include "Components/SkillComponent/Skills/BossSkill/Boss5/ProjectileAttack/Boss5ProjectileAttack.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Util/DebugHelper.h"


UBoss5ProjectileAttack::UBoss5ProjectileAttack()
{
	SkillTag = SkillTags::Boss5ProjectileAttack;

	ProjectileSpawnOffset = FVector(100.0f, 0.0f, 0.0f); // 보스 앞 100 유닛
	ProjectileInitialSpeed = 2000.0f; 
	ProjectileLifeSpan = 5.0f;
}

void UBoss5ProjectileAttack::Execute()
{
	// 스킬 소유자 캐릭터 확인
	if (!OwnerCharacter)
	{
		return;
	}

	if (!ProjectileClass)
	{
		Debug::PrintError(TEXT("UBossProjectileAttack::Execute - ProjectileClass가 할당되지 않았습니다!"));
		// BT 에러 방지
		USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
		if (SkillComponent)
		{
			SkillComponent->EndSkill();
		}
		return;
	}

	FVector SpawnLocation = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * ProjectileSpawnOffset.X +
		OwnerCharacter->GetActorRightVector() * ProjectileSpawnOffset.Y +
		OwnerCharacter->GetActorUpVector() * ProjectileSpawnOffset.Z;
	FRotator SpawnRotation = OwnerCharacter->GetActorRotation(); 

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerCharacter; // 보스를 투사체의 Owner
	SpawnParams.Instigator = OwnerCharacter; // 보스를 투사체의 Instigator
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn; 

	AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);

	if (SpawnedProjectile)
	{
		//속도 설정
		UProjectileMovementComponent* ProjectileMovement = SpawnedProjectile->FindComponentByClass<UProjectileMovementComponent>();
		if (ProjectileMovement)
		{
			ProjectileMovement->InitialSpeed = ProjectileInitialSpeed;
			ProjectileMovement->MaxSpeed = ProjectileInitialSpeed; // 최대 속도도 동일하게 설정
			ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * ProjectileInitialSpeed); // 투사체 자신의 로컬 공간에서 앞으로 이동
			ProjectileMovement->ProjectileGravityScale = 0.0f; // 예시: 중력 없음 (직선 발사)
		}

		//수명 설정
		SpawnedProjectile->SetLifeSpan(ProjectileLifeSpan);

		//사운드 재생
		if (FireSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, OwnerCharacter->GetActorLocation());
		}
	}
	else
	{
		Debug::PrintError(TEXT("UBossProjectileAttack::Execute - 투사체 생성에 실패했습니다!"));
	}

	//스킬 종료
	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	if (SkillComponent)
	{
		SkillComponent->EndSkill();
	}
	else
	{
		Debug::PrintError(TEXT("UBossProjectileAttack::Execute - OwnerCharacter에 SkillComponent가 없습니다!"));
	}
}