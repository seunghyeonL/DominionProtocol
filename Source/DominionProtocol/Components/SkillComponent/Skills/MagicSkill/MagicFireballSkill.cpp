#include "Components/SkillComponent/Skills/MagicSkill/MagicFireBallSkill.h"
#include "Util/GameTagList.h"

UMagicFireBallSkill::UMagicFireBallSkill()
{
	SkillTag = SkillTags::MagicFireBallSkill;
}

void UMagicFireBallSkill::Execute()
{
	Super::Execute();
}

void UMagicFireBallSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const
{
	Super::ApplyAttackToHitActor(HitResult, DeltaTime);
}
