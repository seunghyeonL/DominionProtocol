// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlComponent.h"

#include "ControlComponentUser.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "./States/PlayerControlState.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Effects/PlayerConfusedEffect/PlayerConfusedEffect.h"
#include "Effects/PlayerDeathEffect/PlayerDeathEffect.h"
#include "Effects/PlayerLockOnEffect/PlayerLockOnEffect.h"
#include "Effects/PlayerSilenceEffect/PlayerSilenceEffect.h"
#include "Effects/PlayerStiffnessEffect/PlayerStiffnessEffect.h"
#include "Effects/PlayerStunEffect/PlayerStunEffect.h"
#include "Effects/PlayerUsingSkillEffect/PlayerUsingSkillEffect.h"
#include "InputActionValue.h"

// Sets default values for this component's properties
UPlayerControlComponent::UPlayerControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	
	PlayerControlState = nullptr;
	// bIsComponentReady = false;
	// ...
}

// Called when the game starts
void UPlayerControlComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerControlComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Create Input State
	PlayerControlState = NewObject<UPlayerControlState>(this);
	if (!IsValid(PlayerControlState))
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::InitializeComponent : Invalid PlayerControlState."));
		return;
	}

	auto OwnerCharacter = Cast<ACharacter>(GetOuter());
	if (!IsValid(OwnerCharacter))
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::InitializeComponent : Invalid OwnerCharacter."));
		return;
	}
	PlayerControlState->SetOwnerCharacter(OwnerCharacter);

	// Initialize Effects 
	ActiveControlEffectTags.Reset();
	ControlEffectMap.Empty();

	// Add ControlEffects to ControlEffectMapper 
	ControlEffectMap.Add(EffectTags::Stun, NewObject<UPlayerStunEffect>(this));
	ControlEffectMap.Add(EffectTags::Stiffness, NewObject<UPlayerStiffnessEffect>(this));   
	ControlEffectMap.Add(EffectTags::Silence, NewObject<UPlayerSilenceEffect>(this));     
	ControlEffectMap.Add(EffectTags::Confused, NewObject<UPlayerConfusedEffect>(this));  
	
	ControlEffectMap.Add(EffectTags::Death, NewObject<UPlayerDeathEffect>(this));
	ControlEffectMap.Add(EffectTags::UsingSkill, NewObject<UPlayerUsingSkillEffect>(this));
	ControlEffectMap.Add(EffectTags::LockOn, NewObject<UPlayerLockOnEffect>(this));

	// Set OwnerCharacter to ControlEffects
	for (auto& [ControlEffectTag, ControlEffect] : ControlEffectMap)
	{
		ControlEffect->SetOwnerCharacter(OwnerCharacter);
	}

	// Notify Component Ready
	// if (OnComponentReady.IsBound())
	// {
	// 	OnComponentReady.Execute();
	// }
	// bIsComponentReady = true;
}


// Called every frame
void UPlayerControlComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	for (auto ActiveEffectTag : ActiveControlEffectTags.GetGameplayTagArray())
	{
		ControlEffectMap[ActiveEffectTag]->Tick(DeltaTime);
	}
}

void UPlayerControlComponent::ActivateControlEffect(const FGameplayTag& ControlEffectTag)
{
	if (ControlEffectTag.MatchesTag(EffectTags::UsingSkill))
	{
		if (auto ControlEffect = ControlEffectMap.Find(EffectTags::UsingSkill))
		{
			if (auto UsingSkillEffect = Cast<UPlayerUsingSkillEffect>(*ControlEffect))
			{
				UsingSkillEffect->SetControlEffectTag(ControlEffectTag);
				(*ControlEffect)->Activate();
			}
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffect : UsingSkill Tag Not Initialized in Mapper."));
		}
		
		return;
	}
	
	if (auto ControlEffect = ControlEffectMap.Find(ControlEffectTag))
	{
		(*ControlEffect)->Activate();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffect : Tag Not Initialized in Mapper."));
	}
}

void UPlayerControlComponent::ActivateControlEffect(const FGameplayTag& ControlEffectTag, float Duration)
{
	if (ControlEffectTag.MatchesTag(EffectTags::UsingSkill))
	{
		if (auto ControlEffect = ControlEffectMap.Find(EffectTags::UsingSkill))
		{
			if (auto UsingSkillEffect = Cast<UPlayerUsingSkillEffect>(*ControlEffect))
			{
				UsingSkillEffect->SetControlEffectTag(ControlEffectTag);
				(*ControlEffect)->Activate(Duration);
			}
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffect : UsingSkill Tag Not Initialized in Mapper."));
		}
		
		return;
	}
	
	if (auto ControlEffect = ControlEffectMap.Find(ControlEffectTag))
	{
		(*ControlEffect)->Activate(Duration);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffectWithDuration : Tag Not Initialized in Mapper."));
	}
}

void UPlayerControlComponent::DeactivateControlEffect(const FGameplayTag& ControlEffectTag)
{
	if (ControlEffectTag.MatchesTag(EffectTags::UsingSkill))
	{
		if (auto ControlEffect = ControlEffectMap.Find(EffectTags::UsingSkill))
		{
			if (auto UsingSkillEffect = Cast<UPlayerUsingSkillEffect>(*ControlEffect))
			{
				UsingSkillEffect->SetControlEffectTag(ControlEffectTag);
				(*ControlEffect)->Deactivate();
			}
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlComponent::DeactivateControlEffect : UsingSkill Tag Not Initialized in Mapper."));
		}
		
		return;
	}
	
	if (auto ControlEffect = ControlEffectMap.Find(ControlEffectTag))
	{
		(*ControlEffect)->Deactivate();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::DeactivateControlEffect : Tag Not Initialized in Mapper."));
	}
}

// Input Binding Functions
void UPlayerControlComponent::Move(const FInputActionValue& Value)
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Move(Value);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Move : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Look(const FInputActionValue& Value)
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Look(Value);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Look : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Dash()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Dash();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Dash : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Sprint()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Sprint();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Sprint : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Parry()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Parry();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Parry : Invalid ControlState."));
	}
}

void UPlayerControlComponent::BaseAttack()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->BaseAttack();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::BaseAttack : Invalid ControlState."));
	}
}

void UPlayerControlComponent::WeaponSkill()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->WeaponSkill();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::WeaponSkill : Invalid ControlState."));
	}
}

void UPlayerControlComponent::MagicSkill()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->MagicSkill();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::MagicSkill : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Interact()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Interact();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Interact : Invalid ControlState."));
	}
}

void UPlayerControlComponent::LockOn()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->LockOn();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::LockOn : Invalid ControlState."));
	}
}

