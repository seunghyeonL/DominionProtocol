#include "SkillComponent.h"
#include "SkillComponentUser.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "SkillComponentInitializeData.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Gameframework/Character.h"
#include "Util/DebugHelper.h"

USkillComponent::USkillComponent()
{
    bWantsInitializeComponent = true;
    ComboResetDelay = 1.f;
}

void USkillComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
    Super::OnComponentDestroyed(bDestroyingHierarchy);

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(ResetComboTimer);
    }
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
    for (const auto& [SkillGroupTag, SkillGroupData] : InitializeData.SkillGroupInitializeDatas)
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        check(IsValid(OwnerCharacter));

        FSkillGroup SkillGroup;
        for (const auto& SkillClass : SkillGroupData)
        {
            if (UBaseSkill* Skill = NewObject<UBaseSkill>(this, SkillClass))
            {
                Skill->Initialize(OwnerCharacter);
                SkillGroup.Skills.Add(Skill);
            }
            else
            {
                FString Msg = FString::Printf(TEXT("UStatusComponent::InitializeStatusComponent : Create %s."), *SkillGroupTag.ToString());
                Debug::PrintError(Msg);
            }
        }
        SkillGroupMap.Add(SkillGroupTag, SkillGroup);
    }
}

void USkillComponent::ExecuteSkill(const FGameplayTag& SkillGroupTag)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    check(IsValid(OwnerCharacter));

    auto StatusComponentUser = Cast<IStatusComponentUser>(GetOuter());
    check(StatusComponentUser);

    auto StatusComponent = StatusComponentUser->GetStatusComponent();
    check(IsValid(StatusComponent));
    
    check(SkillGroupTag.IsValid());
    
    if (FSkillGroup* SkillGroup = SkillGroupMap.Find(SkillGroupTag))
    {
        const TArray<UBaseSkill*>& Skills = SkillGroup->Skills;
        int32& ComboIdx = SkillGroup->ComboIdx;

        if (Skills.IsValidIndex(ComboIdx))
        {
            UBaseSkill* Skill = Skills[ComboIdx];
            if (IsValid(Skill))
            {
                SetCurrentSkill(Skill);

                // Check to use Stamina
                if (float CurrentStamina = StatusComponent->GetStat(StatTags::Stamina); CurrentStamina > 0.f)
                {
                    // Check to have enough Stamina 
                    if (CurrentStamina < Skill->GetStamina())
                    {
                        return;
                    }

                    // Use Stamina
                    StatusComponent->ConsumeStamina(Skill->GetStamina());
                }
                
                Skill->Execute(); // 해당 스킬 실행
                if (OnSkillStart.IsBound())
                {
                    OnSkillStart.Execute(Skill->GetControlEffectTag());
                }

                if (CurrentSkillGroupTag.IsValid() && CurrentSkillGroupTag != SkillGroupTag)
                {
                    // 공격이 바뀔 경우 바로 콤보 초기화
                    if (FSkillGroup* CurrentSkillGroup = SkillGroupMap.Find(CurrentSkillGroupTag))
                    {
                        CurrentSkillGroup->ComboIdx = 0;
                    }
                }

                CurrentSkillGroupTag = SkillGroupTag;

                GetWorld()->GetTimerManager().ClearTimer(ResetComboTimer);

                // 콤보 공격일 경우, 다음 실행을 위해 인덱스를 증가시킴
                ComboIdx = (ComboIdx + 1) % SkillGroup->Skills.Num();
            }
        }
        else
        {
            Debug::PrintError(TEXT("USkillComponent::ExecuteSkill : Invalid SkillIndex in SkillGroup"));
        }
    }
    else
    {
        Debug::PrintError(TEXT("USkillComponent::ExecuteSkill : SkillGroupTag is not set in SkillGroupMap"));
    }
}

void USkillComponent::EndSkill()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (Character)
    {
        USkeletalMeshComponent* Mesh = Character->GetMesh();

        if (Mesh)
        {
            UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

            if (AnimInstance)
            {
                UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();

                if (CurrentMontage)
                {
                    AnimInstance->Montage_SetPlayRate(CurrentMontage, CurrentMontage->RateScale * 2);
                }
            }
        }
    }

    if (OnSkillEnd.IsBound())
    {
        OnSkillEnd.Execute(CurrentSkill->GetControlEffectTag());
    }

    TWeakObjectPtr<ThisClass> WeakThis(this);
    GetWorld()->GetTimerManager().SetTimer(
        ResetComboTimer,
        [WeakThis]()
        {
            if (!WeakThis.IsValid())
            {
                return;
            }

            if (FSkillGroup* SkillGroup = WeakThis->SkillGroupMap.Find(WeakThis->CurrentSkillGroupTag))
            {
                SkillGroup->ComboIdx = 0;
            }
        },
        ComboResetDelay,
        false
    );
}

// Debug::PrintLog(TEXT(" "));
