#include "Components/SkillComponent/Skills/FireBallSkill.h"
#include "Util/GameTagList.h"

UFireBallSkill::UFireBallSkill()
{
	SkillTag = SkillTags::FireBallSkill;
}

void UFireBallSkill::Execute()
{
	Super::Execute();
}

void UFireBallSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime) const
{
	Super::ApplyAttackToHitActor(HitResult, DeltaTime);
}
