#include "SkillComponent.h"
#include "SkillComponentUser.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Gameframework/Character.h"
#include "SkillComponentInitializeData.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Util/DebugHelper.h"
#include "AI/AIControllers/BaseAIController.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "Skills/TeleportSkill.h"

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

    auto OwnerCharacter = Cast<ACharacter>(GetOuter());
    check(OwnerCharacter);

    if (auto ItemComponent = OwnerCharacter->FindComponentByClass<UItemComponent>())
    {
        ItemComponent->OnPrimaryWeaponChanged.BindUObject(this, &USkillComponent::SetSkills);
    }
    
    if (auto SkillComponentUser = Cast<ISkillComponentUser>(GetOuter()))
    {
        SkillComponentUser->InitializeSkillComponent();
    }
    else
    {
        Debug::PrintError(TEXT("USkillComponent::InitializeComponent : OwnerCharacter Cast Failed."));
    }
}

void USkillComponent::SetSkills(const FSkillComponentInitializeData& InitializeData)
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
                FString Msg = FString::Printf(TEXT("USkillComponent::InitializeSkillComponent : Create %s."), *SkillGroupTag.ToString());
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
            if (!IsValid(Skill))
            {
                return;
            }
            
            if (IsValid(CurrentSkill))
            {
                if (auto ControlComponent = OwnerCharacter->FindComponentByClass<UPlayerControlComponent>())
                {
                    if (!ControlComponent->IsUsingDoubleExecuteSkill())
                    {
                        Debug::Print(TEXT("USkillComponent::ExecuteSkill : Skill Already Executing."));
                        
                        return;
                    }

                    // !~!~!~!~!~!~!
                    // 임시로직
                    if (CurrentSkill->GetSkillTag() != Skill->GetSkillTag())
                    {
                        if (auto TeleportSkill = Cast<UTeleportSkill>(CurrentSkill))
                        {
                            TeleportSkill->SetReadyToTeleport(false);
                            TeleportSkill->SetCanTeleport(false);
                        }

                        if (auto DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
                        {
                            DomiCharacter->TeleportAura->SetVisibility(false);
                            DomiCharacter->CantTeleportAura->SetVisibility(false);
                        }
                
                        EndSkill();
                    }
                    // !~!~!~!~!~!~!
                }
                else
                {
                    Debug::PrintError(TEXT("USkillComponent::ExecuteSkill : ControlComponent is not valid."));
                    return;
                }
            }
            
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

            // 스킬 실행 전, 현재 사용할 스킬로 저장
            SetCurrentSkill(Skill);
            
            // 스킬 실행 전, 스킬 실행 상태로 바꾸기
            if (OnSkillStart.IsBound())
            {
                OnSkillStart.Execute(Skill->GetControlEffectTag());
            }

            // 해당 스킬 실행
            Skill->Execute(); 
           
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
    if (!IsValid(CurrentSkill))
    {
        return;
    }

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

        ABaseAIController* BaseAIController = Cast<ABaseAIController>(Character->GetController());

        if (IsValid(BaseAIController))
        {
            BaseAIController->ClearCachedTask();
        }
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

    // CurrentSkill을 null로 바꾸는 로직을 먼저하기
    // 안그러면 스킬 실행 후 CurrentSkill이 nullptr로 바뀐다. 
    auto CurrentSkillControlEffectTag = CurrentSkill->GetControlEffectTag();
    
    CurrentSkill = nullptr;
    
    if (OnSkillEnd.IsBound())
    {
        OnSkillEnd.Execute(CurrentSkillControlEffectTag);
    }
}

void USkillComponent::StopSkill()
{
    if (!IsValid(CurrentSkill))
    {
        return;
    }
    
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();

    // 캐릭터가 스킬 몽타주를 재생 중인지 확인
    if (AnimInstance && AnimInstance->Montage_IsPlaying(CurrentSkill->GetAnimMontage()))
    {
        Debug::Print(TEXT("USkillComponent::StopSkill : Montage stop."));
        AnimInstance->Montage_Stop(0.1f, CurrentSkill->GetAnimMontage());
    }
    else
    {
        Debug::Print(TEXT("USkillComponent::StopSkill : Montage not stop."));
    }

    FGameplayTagContainer TagContainer;

    CurrentSkill->StopTrace(TagContainer);

    // 콤보 초기화
    // 이부분 그냥 ResetCombo같은 함수로 뺄까요?
    if (FSkillGroup* SkillGroup = SkillGroupMap.Find(CurrentSkillGroupTag))
    {
        SkillGroup->ComboIdx = 0;
    }
    
    auto CurrentSkillControlEffectTag = CurrentSkill->GetControlEffectTag();
    CurrentSkill = nullptr;
    
    // 스킬종료 델리게이트 호출
    if (OnSkillEnd.IsBound())
    {
        OnSkillEnd.Execute(CurrentSkillControlEffectTag);
    }
}

// Debug::PrintLog(TEXT(" "));
