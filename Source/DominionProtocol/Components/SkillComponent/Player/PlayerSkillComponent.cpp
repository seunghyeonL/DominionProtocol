#include "Components/SkillComponent/Player/PlayerSkillComponent.h"
#include "Util/DebugHelper.h"
//#include "Components/Skill.h"

UPlayerSkillComponent::UPlayerSkillComponent()
{

}

void UPlayerSkillComponent::ExecuteSkill(FGameplayTag SkillGroupTag)
{
    Super::ExecuteSkill(SkillGroupTag);

    AActor* Owner = GetOwner();
    check(Owner);

    //if (ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(Owner))
    //{
    //    Debug::PrintLog(TEXT("Owner is DomiCharacter"));
  
    //if (FSkillGroup* SkillGroup = SkillGroups.Find(SkillGroupTag))
    //{
    //    int32 SkillIdx = SkillGroup->CurrentSkillIdx;

    //}
    //    //SkillGroups
    //    Skill→Execute(PlayerCharacter, AttackPower);
    //}
}

/* 
	FSkillGroup BaseAttackGroup;
	BaseAttackGroup.CurrentSkillIdx = 0;
    SkillGroups.Add(SkillGroupTags::BaseAttack, BaseAttackGroup);
*/