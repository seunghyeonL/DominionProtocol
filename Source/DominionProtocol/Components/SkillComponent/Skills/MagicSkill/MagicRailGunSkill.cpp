#include "Components/SkillComponent/Skills/MagicSkill/MagicRailGunSkill.h"
#include "Util/GameTagList.h"

UMagicRailGunSkill::UMagicRailGunSkill()
{
	SkillTag = SkillTags::MagicRailGunSkill;
}

void UMagicRailGunSkill::Execute()
{
	Super::Execute();
}

void UMagicRailGunSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const
{
	Super::ApplyAttackToHitActor(HitResult, DeltaTime);
}
