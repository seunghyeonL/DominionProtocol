#include "Components/SkillComponent/Skills/RailGunSkill.h"
#include "Util/GameTagList.h"

URailGunSkill::URailGunSkill()
{
	SkillTag = SkillTags::MagicRailGunSkill;
}

void URailGunSkill::Execute()
{
	Super::Execute();
}

void URailGunSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const
{
	Super::ApplyAttackToHitActor(HitResult, DeltaTime);
}
