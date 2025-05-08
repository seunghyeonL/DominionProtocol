#include "Components/SkillComponent/AI/AISkillComponent.h"
#include "Util/BattleDataTypes"
#include "Util/DebugHelper.h"
//#include "Components/Skill.h"

UAISkillComponent::UAISkillComponent()
{

}

//void UAISkillComponent::ExecutePatternSkill(FGameplayTag PatternTag)
//{
//}
//
//void UAISkillComponent::ExecuteRandomPattern()
//{
//}

void UAISkillComponent::ExecuteSkill(FGameplayTag SkillGroupTag)
{
	Super::ExecuteSkill(SkillGroupTag);

	AActor* Owner = GetOwner();
	check(Owner);

	//if (ADomiBaseEnemy* EnemyCharacter = Cast<ADomiBaseEnemy>(Owner))
	//{
	//	Debug::PrintLog(TEXT("Owner is DomiBaseEnemy"));
		Debug::PrintLog(TEXT("AttackData에서 스탯에 따라 Damage 바꿔주는 로직"));
		int Index = SkillGroups[SkillGroupTag].CurrentSkillIdx;
		SkillGroups[SkillGroupTag].SkillGroup[Index].Excute(PlayerCharacter, AttackPower);
	//    Skill→Execute(PlayerCharacter, AttackPower);
	//}
}

void UAISkillComponent::BeginPlay()
{
}