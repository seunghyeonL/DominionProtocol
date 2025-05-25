#include "Components/SkillComponent/Skills/RailGunSkill.h"
#include "RailGunSkill.h"

URailGunSkill::URailGunSkill()
{
	TotalProjectileCount = 1;
	LaunchInterval = 0.1;
}


void URailGunSkill::Execute()
{
	Super::Execute();
}

void URailGunSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime)
{
	Super::ApplyAttackToHitActor(HitResult, DeltaTime);
}
