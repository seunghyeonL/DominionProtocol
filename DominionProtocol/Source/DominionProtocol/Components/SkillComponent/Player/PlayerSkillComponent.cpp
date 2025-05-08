#include "Components/SkillComponent/Player/PlayerSkillComponent.h"
#include "Util/DebugHelper.h"
//#include "Components/Skill.h"
UPlayerSkillComponent::UPlayerSkillComponent()
{

}

void UPlayerSkillComponent::ExecuteSkill(FGameplayTag SkillGroupTag)
{
    AActor* Owner = GetOwner();
    if (!Owner) return;

    //if (ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(Owner))
    //{
    //    Debug::PrintLog(TEXT("Owner is DomiCharacter"));
    //    //SkillGroups
    //    Skill→Execute(AttackPower);
    //}
}

/* 
	FSkillGroup BaseAttackGroup;
	BaseAttackGroup.CurrentSkillIdx = 0;
*/