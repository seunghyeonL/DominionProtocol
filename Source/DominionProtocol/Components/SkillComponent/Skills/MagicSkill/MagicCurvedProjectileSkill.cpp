#include "Components/SkillComponent/Skills/MagicSkill/MagicCurvedProjectileSkill.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Interface/PawnTagInterface.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"

void UMagicCurvedProjectileSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UMagicCurvedProjectileSkill::Execute()
{
	UpdateTarget();

	Super::Execute();
}

void UMagicCurvedProjectileSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const
{
	// Super::ApplyAttackToHitActor(HitResult, DeltaTime);
	// 투사체에서 처리하도록
}

bool UMagicCurvedProjectileSkill::CheckParry(AActor* HitActor) const
{
	return false;
}

void UMagicCurvedProjectileSkill::UpdateTarget()
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

void UMagicCurvedProjectileSkill::SetPlayerAsTarget()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	check(Player);
	TargetActor = Player;
}
