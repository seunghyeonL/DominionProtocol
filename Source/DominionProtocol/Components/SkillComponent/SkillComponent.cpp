#include "SkillComponent.h"
#include "SkillComponentUser.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "SkillComponentInitializeData.h"
#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Gameframework/Character.h"
#include "Util/DebugHelper.h"

USkillComponent::USkillComponent()
{
    bWantsInitializeComponent = true;
    PrimaryComponentTick.bCanEverTick = true;
    ComboResetDelay = 1.f;
}

void USkillComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsValid(CurrentSkill))
    {
        CurrentSkill->Tick(DeltaTime);
    }
}

void USkillComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);

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

        if (auto ControlComponentUser = Cast<IControlComponentUser>(OwnerCharacter))
        {
            // Cashed Movement Vector
            FVector LastInputVector = ControlComponentUser->GetLastMovementVector();
            
            if (!ControlComponentUser->GetActiveControlEffectTags().HasTag(EffectTags::LockOn)) 
            {
                // LockOn상태가 아닐때 Rotation돌리는 로직
                if (!LastInputVector.IsNearlyZero())
                {
                    OwnerCharacter->SetActorRotation(LastInputVector.Rotation());
                }
            }
            else if (SkillGroupTag.MatchesTag(SkillGroupTags::Dash))
            {
                // LockOn이어도 Dash 쓸때는 로테이션 돌리기
                OwnerCharacter->SetActorRotation(LastInputVector.Rotation());
            }
        }

        if (Skills.IsValidIndex(ComboIdx))
        {
            UBaseSkill* Skill = Skills[ComboIdx];
            if (IsValid(Skill))
            {
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
                
                SetCurrentSkill(Skill);
                Skill->Execute(); // 해당 스킬 실행
                if (OnSkillStart.IsBound())
                {
                    OnSkillStart.Execute(Skill->GetControlEffectTag());
                }

                // 공격이 바뀔 경우 바로 콤보 초기화
                if (CurrentSkillGroupTag.IsValid() && CurrentSkillGroupTag != SkillGroupTag)
                {
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

    if (IsValid(Character))
    {
        USkeletalMeshComponent* Mesh = Character->GetMesh();

        if (IsValid(Mesh))
        {
            UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

            if (IsValid(AnimInstance))
            {
                UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();
                
                if (IsValid(CurrentMontage))
                {
                    AnimInstance->Montage_SetPlayRate(CurrentMontage, CurrentMontage->RateScale * 5);
                }
            }
        }
    }

    if (OnSkillEnd.IsBound() && IsValid(CurrentSkill))
    {
        OnSkillEnd.Execute(CurrentSkill->GetControlEffectTag());
    }
    CurrentSkill = nullptr;

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

void USkillComponent::StopSkill()
{
    if (IsValid(CurrentSkill))
    {
        ACharacter* Character = Cast<ACharacter>(GetOwner());
        UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

        // 캐릭터가 스킬 몽타주를 재생 중인지 확인
        if (AnimInstance && AnimInstance->Montage_IsPlaying(CurrentSkill->GetAnimMontage()))
        {
            AnimInstance->Montage_Stop(0.1f, CurrentSkill->GetAnimMontage());
        }

        if (OnSkillEnd.IsBound())
        {
            OnSkillEnd.Execute(CurrentSkill->GetControlEffectTag());
        }

        CurrentSkill = nullptr;
    }
}

// Debug::PrintLog(TEXT(" "));
