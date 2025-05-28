#include "Components/SkillComponent/Skills/Notifies/AnimNotify_SpawnProjectile.h"
#include "Util/GameTagList.h"
#include "Kismet/GameplayStatics.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"
#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"

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

	ProjectileSkill = Cast<UCurvedProjectileSkill>(CurrentSkill);
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
	if (!CachedMeshComp) return;

	FVector SpawnLocation = OwnerCharacter->GetActorTransform().TransformPosition(Offset);
	FRotator SpawnRotation = FRotator::ZeroRotator;

	UWorld* World = CachedMeshComp->GetWorld();
	if (!World) return;

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	if (!BaseGameState) return;

	if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
	{
		if (!IsValid(SkillData->CurvedProjectileClass)) return;

		ACurvedProjectile* CurvedProjectile = World->SpawnActor<ACurvedProjectile>(SkillData->CurvedProjectileClass, SpawnLocation, SpawnRotation);

		if (SkillData->Sound[0])
		{
			UGameplayStatics::PlaySoundAtLocation(this, SkillData->Sound[0], SpawnLocation);
		}

		if (CurvedProjectile)
		{
			CurvedProjectile->SkillOwner = ProjectileSkill;
			CurvedProjectile->SkillTag = SkillTag;
			CurvedProjectile->SetLaunchPath(OwnerCharacter, TargetActor);
		}
	}
}