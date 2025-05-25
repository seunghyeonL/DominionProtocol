#include "Components/SkillComponent/Skills/FireballSkill.h"
#include "FireballSkill.h"

UFireballSkill::UFireballSkill()
{
	TotalProjectileCount = 3;
	LaunchInterval = 0.3;
}

void UFireballSkill::Execute()
{
	Super::Execute();
}

void UFireballSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime)
{
	Super::ApplyAttackToHitActor(HitResult, DeltaTime);
}
