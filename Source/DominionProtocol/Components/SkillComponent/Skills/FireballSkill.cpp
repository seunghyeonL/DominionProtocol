#include "Components/SkillComponent/Skills/FireBallSkill.h"
#include "Util/GameTagList.h"

UFireBallSkill::UFireBallSkill()
{
	SkillTag = SkillTags::FireBallSkill;
	TotalProjectileCount = 3;
	LaunchInterval = 0.1;
}

void UFireBallSkill::Execute()
{
	Super::Execute();
}

void UFireBallSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime)
{
	Super::ApplyAttackToHitActor(HitResult, DeltaTime);
}
