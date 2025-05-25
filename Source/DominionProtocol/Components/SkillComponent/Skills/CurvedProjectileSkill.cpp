#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"
#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
//#include "DomiFramework/ObjectPooling/ObjectPoolSubsystem.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/PawnTagInterface.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Player/Damagable.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"


UCurvedProjectileSkill::UCurvedProjectileSkill()
{
}

void UCurvedProjectileSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UCurvedProjectileSkill::Execute()
{
	Super::Execute();

	ProjectileIndexToLaunch = 0;

	UpdateTarget();

	//ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	//if (!IsValid(ObjectPoolSubsystem)) return;

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	CurrentSkill = SkillComponent->GetCurrentSkill();

	// 타이머로 투사체 순차 생성
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UCurvedProjectileSkill::ProjectileFromPool, LaunchInterval, true);

}

void UCurvedProjectileSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime)
{
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))	return;

	UWorld* World = GetWorld();

	if (!IsValid(World)) return;

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

	if (!IsValid(BaseGameState)) return;

	UStatusComponent* StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();

	if (!IsValid(StatusComponent)) return;
	float AttackPower = StatusComponent->GetStat(StatTags::AttackPower);

	FAttackData AttackData;

	check(OwnerCharacter);

	AttackData.Damage = GetFinalAttackData(AttackPower);

	AttackData.Instigator = OwnerCharacter;
	AttackData.Effects = Effects;

	if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
	{
		AttackData.LaunchVector = HitActor->GetActorLocation() - OwnerCharacter->GetActorLocation();

		AttackData.LaunchVector.Normalize();

		IDamagable::Execute_OnAttacked(HitActor, AttackData);
	}
}

void UCurvedProjectileSkill::UpdateTarget()
{
	if (!IsValid(OwnerCharacter)) return;

	if (OwnerCharacter->GetClass()->ImplementsInterface(UPawnTagInterface::StaticClass()))
	{
		FGameplayTag Tag = IPawnTagInterface::Execute_GetPawnTag(OwnerCharacter);
		if (Tag == PawnTags::Player)
		{
			// Lock On
			UPlayerControlComponent* ControlComp = OwnerCharacter->FindComponentByClass<UPlayerControlComponent>();
			check(ControlComp);
			TargetActor = ControlComp->GetLockOnTargetActor();

			if (!TargetActor)
			{
				// Not Lock On
				TargetActor = nullptr;
			}
		}
		else
		{
			// Enemy
			SetPlayerAsTarget();
		}
	}
}

void UCurvedProjectileSkill::ProjectileFromPool()
{
	if (ProjectileIndexToLaunch >= TotalProjectileCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
		return;
	}

	ProjectileIndexToLaunch++;

	int32 Index = ProjectileIndexToLaunch - 1;
	Debug::PrintError(TEXT("current Index : '") + FString::FromInt(Index) + TEXT("'"));

	FVector Offset = FVector(0.0f, 50.0f, 150.0f);
	FVector SpawnLocation = OwnerCharacter->GetActorTransform().TransformPosition(Offset);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// 풀에서 투사체 가져오기
	//CurvedProjectile = Cast<ACurvedProjectile>(ObjectPoolSubsystem->SpawnActorFromPool(SkillTag, SpawnLocation, SpawnRotation, OwnerCharacter));

	//CurvedProjectileType = GetWorld()->SpawnActor<ACurvedProjectile>(
	//	ACurvedProjectile::StaticClass(),
	//	SpawnLocation,
	//	SpawnRotation);

	UWorld* World = GetWorld();
	check(World);

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	check(BaseGameState);

	if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
	{
		CurvedProjectileClass = SkillData->CurvedProjectileClass;
		if (!IsValid(CurvedProjectileClass)) return;

		CurvedProjectile = GetWorld()->SpawnActor<ACurvedProjectile>(
			CurvedProjectileClass,
			SpawnLocation,
			SpawnRotation
		);
		if (CurvedProjectile)
		{
			CurvedProjectile->SkillOwner = this;
			CurvedProjectile->SkillTag = SkillTag;
			CurvedProjectile->SetLaunchPath(OwnerCharacter, TargetActor);
		}
	}	
}

void UCurvedProjectileSkill::SetPlayerAsTarget()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(Player);
	TargetActor = Player;
}
