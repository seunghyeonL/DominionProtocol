#include "SkillComponent.h"
#include "SkillComponentUser.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "SkillComponentInitializeData.h"
#include "Gameframework/Character.h"
#include "Util/DebugHelper.h"

USkillComponent::USkillComponent()
{
    bWantsInitializeComponent = true;
    ComboResetDelay = 1.f;
}

USkillComponent::~USkillComponent()
{
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
                Skill->Initialize();
                SkillGroup.Skills.Add(Skill);

                UPlayerControlComponent* PlayerControlComponent = OwnerCharacter->FindComponentByClass<UPlayerControlComponent>();

                // 스킬 시작 및 끝나는 시점 델리게이트 바인딩
                // Skill->OnSkillStart.BindUObject(PlayerControlComponent, &UPlayerControlComponent::UsingSkillEffectDeactivate);
                // Skill->OnSkillEnd.BindUObject(PlayerControlComponent, &UPlayerControlComponent::UsingSkillEffectActivate);
                //
                // Debug::Print(TEXT("USkillComponent::OnSkillStart is Bound (Player)"));
                // Debug::Print(TEXT("USkillComponent::OnSkillEnd is Bound (Player)"));

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
                
                Skill->Execute(OwnerCharacter); // 해당 스킬 실행
                if (OnSkillStart.IsBound())
                {
                    OnSkillStart.Execute(Skill->GetControlEffectTag());
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
                // AnimInstance로 원하는 작업 수행

                UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage();

                if (CurrentMontage)
                {
                    // 현재 실행 중인 몽타주가 있음

                    AnimInstance->Montage_SetPlayRate(CurrentMontage, CurrentMontage->RateScale * 2);
                }
            }
        }
    }

    if (OnSkillEnd.IsBound())
    {
        OnSkillEnd.Execute(CurrentSkill->GetSkillTag());
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