#include "Components/SkillComponent/Skills/FireBallSkill.h"
#include "Util/GameTagList.h"

UFireBallSkill::UFireBallSkill()
{
	SkillTag = SkillTags::FireBallSkill;
	TotalProjectileCount = 3;
	LaunchInterval = 0.1;
	// Offset = FVector(0.0f, 50.0f, 150.0f);
	Offset = FVector(0.0f, 0.0f, 80.0f);
}

void UFireBallSkill::Execute()
{
	Super::Execute();
}

void UFireBallSkill::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime)
{
	Super::ApplyAttackToHitActor(HitResult, DeltaTime);
}
