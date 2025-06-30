#include "Components/SkillComponent/Skills/BossSkill/Boss5/ProjectileAttack/Boss5ProjectileAttack.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AI/AIControllers/Boss5AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Util/DebugHelper.h"


UBoss5ProjectileAttack::UBoss5ProjectileAttack()
{
	SkillTag = SkillTags::Boss5ProjectileAttack;

	ProjectileSpawnOffset = FVector(100.0f, 0.0f, 0.0f); // 보스 앞 100 유닛
	ProjectileInitialSpeed = 500.0f; 
	ProjectileLifeSpan = 5.0f;
}

void UBoss5ProjectileAttack::Execute()
{
	// 스킬 소유자 캐릭터 확인
	if (!OwnerCharacter)
	{
		return;
	}
	AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
	if (!AIController)
	{
		Debug::PrintError(TEXT("UBoss5ProjectileAttack::Execute - OwnerCharacter의 AIController를 찾을 수 없습니다!"));
		// 스킬 종료하여 BT가 멈추지 않도록
		USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
		if (SkillComponent) { SkillComponent->EndSkill(); }
		return;
	}
	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (!BBComp)
	{
		Debug::PrintError(TEXT("UBoss5ProjectileAttack::Execute - AIController의 BlackboardComponent를 찾을 수 없습니다!"));
		// 스킬 종료하여 BT가 멈추지 않도록
		USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
		if (SkillComponent) { SkillComponent->EndSkill(); }
		return;
	}
	TargetCharacter = Cast<ACharacter>(BBComp->GetValueAsObject(FName("TargetActor")));
	if (!TargetCharacter)
	{
		Debug::PrintError(TEXT("UBoss5ProjectileAttack::Execute - Blackboard에서 TargetActor를 찾을 수 없습니다!"));
		// 스킬 종료하여 BT가 멈추지 않도록
		USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
		if (SkillComponent)
		{ 
			SkillComponent->EndSkill();
		}
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

	FVector SpawnLocation = OwnerCharacter->GetActorLocation() + 
		OwnerCharacter->GetActorForwardVector() * ProjectileSpawnOffset.X +
		OwnerCharacter->GetActorRightVector() * ProjectileSpawnOffset.Y +
		OwnerCharacter->GetActorUpVector() * ProjectileSpawnOffset.Z;

	FVector WorldDirectionToTarget = (TargetCharacter->GetActorLocation() - SpawnLocation).GetSafeNormal();
	FRotator SpawnRotation = WorldDirectionToTarget.Rotation();

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
			ProjectileMovement->Velocity = WorldDirectionToTarget * ProjectileInitialSpeed; // 투사체 자신의 로컬 공간에서 앞으로 이동
			ProjectileMovement->ProjectileGravityScale = 0.0f;
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