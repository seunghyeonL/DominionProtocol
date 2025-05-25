#include "Components/SkillComponent/Skills/RailGunSkill.h"
#include "Util/GameTagList.h"

URailGunSkill::URailGunSkill()
{
	SkillTag = SkillTags::RailGunSkill;
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
