// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlComponent.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "./States/PlayerControlState.h"
#include "Effects/PlayerConfusedEffect/PlayerConfusedEffect.h"
#include "Effects/PlayerDeathEffect/PlayerDeathEffect.h"
#include "Effects/PlayerSilenceEffect/PlayerSilenceEffect.h"
#include "Effects/PlayerStiffnessEffect/PlayerStiffnessEffect.h"
#include "Effects/PlayerStunEffect/PlayerStunEffect.h"
#include "Effects/PlayerUsingDashEffect/PlayerUsingDashEffect.h"


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
		Debug::PrintError(TEXT("UPlayerControlComponent::BeginPlay : Invalid PlayerControlState."));
		return;
	}

	auto OwnerCharacter = Cast<ACharacter>(GetOuter());
	if (!IsValid(OwnerCharacter))
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::BeginPlay : Invalid OwnerCharacter."));
		return;
	}
	PlayerControlState->SetOwnerCharacter(OwnerCharacter);

	// Initialize Effects 
	ActiveControlEffectTags.Reset();
	ControlEffectMapper.Empty();

	// Add ControlEffects to ControlEffectMapper 
	ControlEffectMapper.Add(EffectTags::Stun, NewObject<UPlayerStunEffect>(this));
	ControlEffectMapper.Add(EffectTags::Stiffness, NewObject<UPlayerStiffnessEffect>(this));   
	ControlEffectMapper.Add(EffectTags::Silence, NewObject<UPlayerSilenceEffect>(this));     
	ControlEffectMapper.Add(EffectTags::Confused, NewObject<UPlayerConfusedEffect>(this));  
	
	ControlEffectMapper.Add(EffectTags::Death, NewObject<UPlayerDeathEffect>(this));
	// ControlEffectMapper.Add(EffectTags::UsingSkill, NewObject<UUsingSkillEffect>(this));
	// ControlEffectMapper.Add(EffectTags::UsingParry, NewObject<UUsingParryEffect>(this));
	ControlEffectMapper.Add(EffectTags::UsingDash, NewObject<UPlayerUsingDashEffect>(this));

	// Set OwnerCharacter to ControlEffects
	for (auto& [ControlEffectTag, ControlEffect] : ControlEffectMapper)
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
}

void UPlayerControlComponent::ActivateControlEffect(const FGameplayTag& ControlEffectTag)
{
	if (!ControlEffectMapper.Contains(ControlEffectTag))
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffect : Tag Not Initialized in Mapper."));
		return;
	}

	if (auto ControlEffect = ControlEffectMapper[ControlEffectTag])
	{
		ControlEffect->Activate();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayerControlComponent::ActivateControlEffect : Invalid ControlEffect."))
	}
}

void UPlayerControlComponent::ActivateControlEffectWithDuration(const FGameplayTag& ControlEffectTag, float Duration)
{
	if (!ControlEffectMapper.Contains(ControlEffectTag))
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffect : Tag Not Initialized in Mapper."));
		return;
	}

	if (auto ControlEffect = ControlEffectMapper[ControlEffectTag])
	{
		ControlEffect->Activate(Duration);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayerControlComponent::ActivateControlEffect : Invalid ControlEffect."))
	}
}

void UPlayerControlComponent::DeactivateControlEffect(const FGameplayTag& ControlEffectTag)
{
	if (!ControlEffectMapper.Contains(ControlEffectTag))
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::DeactivateControlEffect : Tag Not Initialized in Mapper."));
		return;
	}

	if (auto ControlEffect = ControlEffectMapper[ControlEffectTag])
	{
		ControlEffect->Deactivate();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UPlayerControlComponent::DeactivateControlEffect : Invalid ControlEffect."))
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

