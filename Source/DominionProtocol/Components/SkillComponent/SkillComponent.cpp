#include "SkillComponent.h"
#include "SkillComponentUser.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "SkillComponentInitializeData.h"
#include "Gameframework/Character.h"
#include "Util/DebugHelper.h"

USkillComponent::USkillComponent()
{
    bWantsInitializeComponent = true;
}

void USkillComponent::InitializeComponent()
{
    Super::InitializeComponent();

    if (auto OnwerCharacter = Cast<ISkillComponentUser>(GetOuter()))
    {
        OnwerCharacter->InitializeSkillComponent();
    }
    else
    {
        Debug::PrintError(TEXT("USkillComponent::InitializeComponent : OwnerCharacter Cast Failed."));
    }
}

void USkillComponent::InitializeSkillComponent(const FSkillComponentInitializeData& InitializeData)
{
    //const auto& [SkillGroups] = InitializeData;

    //for (auto [SkillTag, SkillGroup] : SkillGroups)
    //{
    //    SkillGroupsMap.Add(SkillTag, SkillGroup);
    //}

    for (const TPair<FGameplayTag, FSkillGroup>& Pair : InitializeData.SkillGroups)
    {
        SkillGroupsMap.Add(Pair.Key, Pair.Value);
    }
}

void USkillComponent::ExecuteSkill(const FGameplayTag& SkillGroupTag)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    check(OwnerCharacter);

    if (FSkillGroup* SkillGroupData = SkillGroupsMap.Find(SkillGroupTag))
    {
        const TArray<TSubclassOf<UBaseSkill>>& SkillClasses = SkillGroupData->SkillGroup;
        int32 CurrentIdx = SkillGroupData->CurrentSkillIdx;

        if (SkillClasses.IsValidIndex(CurrentIdx))
        {
            TSubclassOf<UBaseSkill> SkillClass = SkillClasses[CurrentIdx];
            if (SkillClass)
            {
                UBaseSkill* ActiveSkillInstance = nullptr;
                
                // 현재 캐싱된 스킬이 동일한 클래스라면 그대로 사용
                if (CurrentSkill && CurrentSkill->GetClass() == SkillClass)
                {
                    ActiveSkillInstance = CurrentSkill;
                }
                else
                {
                    // UBaseSkill 인스턴스 생성
                    ActiveSkillInstance = NewObject<UBaseSkill>(this, SkillClass);
                    if (ActiveSkillInstance)
                    {
                        SetCurrentSkill(ActiveSkillInstance);
                    }
                }

                if (ActiveSkillInstance)
                {
                    ActiveSkillInstance->Execute(OwnerCharacter); // 해당 스킬 실행

                    // 콤보 공격일 경우, 다음 실행을 위해 인덱스를 증가시킴
                    SkillGroupData->CurrentSkillIdx = (SkillGroupData->CurrentSkillIdx + 1) % SkillGroupData->SkillGroup.Num();
                }                
            }
        }
        else
        {
            Debug::PrintError(TEXT("Invalid SkillIndex in SkillGroupTag"));
        }
    }
    else
    {
        Debug::PrintError(TEXT("SkillGroupTag passed in was not found"));
    }
}

// Debug::PrintLog(TEXT(" "));