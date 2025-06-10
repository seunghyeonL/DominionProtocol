#include "Components/SkillComponent/Skills/Notifies/AnimNotify_SpawnProjectile.h"
#include "Util/GameTagList.h"
#include "Kismet/GameplayStatics.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/Skills/MagicSkill/MagicCurvedProjectileSkill.h"
#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Util/DebugHelper.h"

void UAnimNotify_SpawnProjectile::NotifyBegin(USkeletalMeshComponent* MeshComp,	UAnimSequenceBase* Animation, float TotalDuration)
{
	if (!IsValid(MeshComp)) return;

	CachedMeshComp = MeshComp;
	OwnerCharacter = MeshComp->GetOwner();
	if (!IsValid(OwnerCharacter)) return;

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	if (!IsValid(SkillComponent)) return;

	UBaseSkill* CurrentSkill = SkillComponent->GetCurrentSkill();
	if (!IsValid(CurrentSkill)) return;

	ProjectileSkill = Cast<UMagicCurvedProjectileSkill>(CurrentSkill);
	if (ProjectileSkill)
	{
		TargetActor = ProjectileSkill->TargetActor;
		SkillTag = CurrentSkill->GetSkillTag();
	}

	ElapsedTime = 0.0f;
	ProjectileIndexToLaunch = 0;
}

void UAnimNotify_SpawnProjectile::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
	if (!CachedMeshComp || !IsValid(OwnerCharacter) || !ProjectileSkill) return;

	ElapsedTime += FrameDeltaTime;

	while (ElapsedTime >= LaunchInterval && ProjectileIndexToLaunch < TotalProjectileCount)
	{
		ElapsedTime -= LaunchInterval;
		ProjectileFromPool();
		ProjectileIndexToLaunch++;
	}
}

void UAnimNotify_SpawnProjectile::ProjectileFromPool()
{
	check(CachedMeshComp);

	if (!Offsets.IsValidIndex(ProjectileIndexToLaunch))
	{
		Debug::PrintError(TEXT("UAnimNotify_SpawnProjectile::ProjectileFromPool : Invalid projectile index."));
		return;
	}

	FVector SpawnLocation = OwnerCharacter->GetActorTransform().TransformPosition(Offsets[ProjectileIndexToLaunch]);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	USkillComponent* SkillComopnent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	if (!IsValid(SkillComopnent))
	{
		Debug::PrintError(TEXT("UAnimNotify_SpawnProjectile::ProjectileFromPool : Invalid SkillComponent."));
		return;
	}

	UBaseSkill* CurrentSkill = SkillComopnent->GetCurrentSkill();
	if (!IsValid(CurrentSkill))
	{
		Debug::PrintError(TEXT("UAnimNotify_SpawnProjectile::ProjectileFromPool : Invalid CurrentSkill.."));
		return;
	}

	UWorld* World = CachedMeshComp->GetWorld();
	if (!World) return;

	if (!IsValid(CurrentSkill->GetCurvedProjectileClass()))
	{
		Debug::PrintError(TEXT("UAnimNotify_SpawnProjectile::ProjectileFromPool : Invalid ProjectileClass."));
		return;
	}

	if (auto CurvedProjectile = World->SpawnActor<ACurvedProjectile>(CurrentSkill->GetCurvedProjectileClass(), SpawnLocation, SpawnRotation))
	{
		const auto& Sounds = CurrentSkill->GetSounds();
		if (!Sounds.IsEmpty() && Sounds.IsValidIndex(0))
		{
			UGameplayStatics::PlaySoundAtLocation(World, Sounds[0], SpawnLocation);
		}

		CurvedProjectile->SkillOwner = ProjectileSkill;
		CurvedProjectile->SkillTag = SkillTag;
		
		FAttackData AttackData;
	
		UStatusComponent* StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();
	
		// if (IsValid(StatusComponent))
		// {
		// 	float AttackPower = StatusComponent->GetStat(StatTags::AttackPower);
		// 	AttackData.Damage = CurrentSkill->GetFinalAttackData();
		// }

		AttackData.Damage = CurrentSkill->GetFinalAttackData();
	
		AttackData.Instigator = OwnerCharacter;
		AttackData.Effects = CurrentSkill->GetEffects();
		AttackData.LaunchVector = FVector::ZeroVector;
	
		CurvedProjectile->AttackData = AttackData;
			
		CurvedProjectile->SetLaunchPath(OwnerCharacter, TargetActor);
	}	
}