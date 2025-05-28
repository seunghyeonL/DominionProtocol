#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Interface/PawnTagInterface.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"

void UCurvedProjectileSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UCurvedProjectileSkill::Execute()
{
	UpdateTarget();

	Super::Execute();
}

void UCurvedProjectileSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const
{
	// Super::ApplyAttackToHitActor(HitResult, DeltaTime);
	// 투사체에서 처리하도록
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

void UCurvedProjectileSkill::SetPlayerAsTarget()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(Player);
	TargetActor = Player;
}
