#include "Components/SkillComponent/AI/AISkillComponent.h"
#include "Util/DebugHelper.h"


UAISkillComponent::UAISkillComponent()
{

}

void UAISkillComponent::ExecuteSkill(FGameplayTag SkillGroupTag)
{
	FSkillGroup BaseAttackGroup;
	BaseAttackGroup.CurrentSkillIdx = 0;
	Patterns.Add(SkillGroupTags::BaseAttack, BaseAttackGroup);

	AActor* Owner = GetOwner();
	if (!Owner) return;

	//if (ADomiBaseEnemy* EnemyCharacter = Cast<ADomiBaseEnemy>(Owner))
	//{
	//	Debug::PrintLog(TEXT("Owner is DomiBaseEnemy"));
		Debug::PrintLog(TEXT("AttackData에서 스탯에 따라 Damage 바꿔주는 로직"));
	//	SkillGroups
	//	Skill→Execute(AttackPower);
	//}
}

void UAISkillComponent::BeginPlay()
{
}

void UAISkillComponent::InitializePatterns()
{
}
