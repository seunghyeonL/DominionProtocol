// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIControllers/Boss4Phase2AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AICharacters/BossMonster/Boss4Enemy.h"
#include "Components/StatusComponent/StatusComponent.h"

ABoss4Phase2AIController::ABoss4Phase2AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultWeaponAttack2Weight = 1;
	DefaultWeaponAttack3Weight = 3;
	DefaultEvadeAttackWeight = 0;
	DefaultRisingWallWeight = 0;
	DefaultSlashWaveWeight = 5;

	WeaponAttack2Increment = 1;
	WeaponAttack3Increment = 2;
	EvadeAttackIncrement = 1;
	RisingWallIncrement = 1;
	SlashWaveIncrement = 0;

	WeaponAttack2CoolDown = 20.f;
	EvadeAttackCoolDown = 20.f;
	RisingWallCoolDown = 70.f;

	bIsActiveWeaponAttack2 = true;
	bIsActiveEvadeAttack = false;	// 체력 90% 이하부터 활성화
	bIsActiveRisingWall = false;	// 체력 80% 이하부터 활성화

	bIsActiveSpawnClone2Phase = false;

	CachedMaxHealth = 0.f;
}

void ABoss4Phase2AIController::BeginPlay()
{
	Super::BeginPlay();

	CurrentWeaponAttack2Weight = DefaultWeaponAttack2Weight;
	CurrentWeaponAttack3Weight = DefaultWeaponAttack3Weight;
	CurrentEvadeAttackWeight = DefaultEvadeAttackWeight;
	CurrentRisingWallWeight = DefaultRisingWallWeight;
	CurrentSlashWaveWeight = DefaultSlashWaveWeight;

	APawn* Boss4Pawn = GetPawn();

	if (IsValid(Boss4Pawn))
	{
		ABoss4Enemy* Boss4 = Cast<ABoss4Enemy>(Boss4Pawn);

		CachedStatusComponent = Boss4->GetStatusComponent();
		CachedMaxHealth = CachedStatusComponent->GetStat(StatTags::MaxHealth);
	}
}

void ABoss4Phase2AIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsActiveSpawnClone2Phase)
	{
		return;
	}

	if (!IsValid(CachedStatusComponent))
	{
		return;
	}

	if (FMath::IsNearlyZero(CachedMaxHealth))
	{
		return;
	}

	if (!IsValid(Blackboard))
	{
		return;
	}

	float CurrentHealth = CachedStatusComponent->GetStat(StatTags::Health);
	float CurrentHealthPercent = CurrentHealth / CachedMaxHealth;

	if (CurrentHealthPercent <= 0.9f && !GetWorldTimerManager().IsTimerActive(EvadeAttackCoolDownTimer))
	{
		bIsActiveEvadeAttack = true;
	}

	if (CurrentHealthPercent <= 0.8f && !GetWorldTimerManager().IsTimerActive(RisingWallCoolDownTimer))
	{
		bIsActiveRisingWall = true;
	}

	if (CurrentHealthPercent > 0.5f)
	{
		return;
	}

	bIsActiveSpawnClone2Phase = true;

	Blackboard->SetValueAsBool(TEXT("bActiveSpawnClone"), true);
}

FGameplayTag ABoss4Phase2AIController::GetAttack()
{
	APawn* Boss4Pawn = GetPawn();

	if (!IsValid(Boss4Pawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABoss4Phase2AIController::GetAttack()] Failed: No Owner"));
		return SkillGroupTags::Boss4WeaponAttack3;
	}

	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABoss4Phase2AIController::GetAttack()] Failed: No Blackboard"));
		return SkillGroupTags::Boss4WeaponAttack3;
	}

	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));

	if (!IsValid(TargetObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABoss4Phase2AIController::GetAttack()] Failed: No TargetActor"));
		return SkillGroupTags::Boss4WeaponAttack3;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (!IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ABoss4Phase2AIController::GetAttack()] Failed: TargetActor Not Actor"));
		return SkillGroupTags::Boss4WeaponAttack3;
	}

	FVector OwnerLocation = Boss4Pawn->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	// Z값 차이를 고려 안 하기 위해 2D
	float Distance = FVector::Dist2D(OwnerLocation, TargetLocation);

	bool bCanWeaponAttack2 = bIsActiveWeaponAttack2 && Distance < 1000.f;
	bool bCanWeaponAttack3 = true;
	bool bCanEvadeAttack = bIsActiveEvadeAttack && Distance < 1000.f;
	bool bCanRisingWall = bIsActiveRisingWall && Distance < 2000.f;
	bool bCanSlashWave = Distance > 1000.f;

	int32 TotalWeight = 0;

	if (bCanWeaponAttack2)
	{
		TotalWeight += CurrentWeaponAttack2Weight;
	}

	if (bCanWeaponAttack3)
	{
		TotalWeight += CurrentWeaponAttack3Weight;
	}

	if (bCanEvadeAttack)
	{
		TotalWeight += CurrentEvadeAttackWeight;
	}

	if (bCanRisingWall)
	{
		TotalWeight += CurrentRisingWallWeight;
	}

	if (bCanSlashWave)
	{
		TotalWeight += CurrentSlashWaveWeight;
	}

	int32 RandomWeight = FMath::RandRange(1, TotalWeight);

	FGameplayTag ReturnTag = SkillGroupTags::Boss4WeaponAttack3;

	if (bCanWeaponAttack2)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentWeaponAttack2Weight)
		{
			ReturnTag = SkillGroupTags::Boss4WeaponAttack2;
			CurrentWeaponAttack2Weight = DefaultWeaponAttack2Weight;
			DeactivateSkill(&bIsActiveWeaponAttack2, WeaponAttack2CoolDown, &WeaponAttack2CoolDownTimer);
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentWeaponAttack2Weight;
			CurrentWeaponAttack2Weight += WeaponAttack2Increment;
		}
	}

	if (bCanWeaponAttack3)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentWeaponAttack3Weight)
		{
			ReturnTag = SkillGroupTags::Boss4WeaponAttack3;
			CurrentWeaponAttack3Weight = DefaultWeaponAttack3Weight;
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentWeaponAttack3Weight;
			CurrentWeaponAttack3Weight += WeaponAttack3Increment;
		}
	}

	if (bCanEvadeAttack)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentEvadeAttackWeight)
		{
			ReturnTag = SkillGroupTags::Boss4EvadeAttack;
			CurrentEvadeAttackWeight = DefaultEvadeAttackWeight;
			DeactivateSkill(&bIsActiveEvadeAttack, EvadeAttackCoolDown, &EvadeAttackCoolDownTimer);
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentEvadeAttackWeight;
			CurrentEvadeAttackWeight += EvadeAttackIncrement;
		}
	}

	if (bCanRisingWall)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentRisingWallWeight)
		{
			ReturnTag = SkillGroupTags::Boss4RisingWall;
			CurrentRisingWallWeight = DefaultRisingWallWeight;
			DeactivateSkill(&bIsActiveRisingWall, RisingWallCoolDown, &RisingWallCoolDownTimer);
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentRisingWallWeight;
			CurrentRisingWallWeight += RisingWallIncrement;
		}
	}

	if (bCanSlashWave)
	{
		if (RandomWeight > 0 && RandomWeight <= CurrentSlashWaveWeight)
		{
			ReturnTag = SkillGroupTags::Boss4SlashWave;
			CurrentSlashWaveWeight = DefaultSlashWaveWeight;
			RandomWeight = -1;
		}
		else
		{
			RandomWeight -= CurrentSlashWaveWeight;
			CurrentSlashWaveWeight += SlashWaveIncrement;
		}
	}

	return ReturnTag;
}

void ABoss4Phase2AIController::DeactivateSkill(bool* IsActive, float CoolDown, FTimerHandle* CoolDownTimerHandle)
{
	*IsActive = false;
	TWeakObjectPtr<ABoss4Phase2AIController> WeakThis(this);

	GetWorld()->GetTimerManager().SetTimer(
		*CoolDownTimerHandle,
		[WeakThis, IsActive]()
		{
			if (!WeakThis.IsValid())
			{
				return;
			}

			*IsActive = true;
		},
		CoolDown,
		false
	);
}
