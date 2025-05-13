// Copyright Epic Games, Inc. All Rights Reserved.

#include "DomiCharacter.h"
#include "Util/DebugHelper.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../InGameController.h"
#include "InputActionValue.h"
#include "Util/BattleDataTypes.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/PlayerControlComponent/States/PlayerControlState.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "Components/StatusComponent/StatusComponentInitializeData.h"
#include "Components/StatusComponent/StatusEffects/AttackDownEffect/AttackDownEffect.h"
#include "Components/StatusComponent/StatusEffects/PoisonEffect/PoisonEffect.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "Components/SkillComponent/Skills/BaseAttack.h"
#include "Components/StatusComponent/StatusEffects/PlayerRunningEffect/PlayerRunningEffect.h"

#include "DomiFramework/GameMode/BaseGameMode.h"


class UPoisonEffect;
//////////////////////////////////////////////////////////////////////////
// ADomiCharacter

ADomiCharacter::ADomiCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Battle Components
	ControlComponent = CreateDefaultSubobject<UPlayerControlComponent>(TEXT("ControlComponent"));
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

AActor* ADomiCharacter::GetCurrentInteractableObject() const
{
	if (InteractableObject == nullptr)
	{
		Debug::Print("ADomiCharacter::GetCurrentInteractableObject : InteractableObject == nullptr");
		return nullptr;
	}
	else
	{
		return InteractableObject;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADomiCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (auto PlayerController = Cast<AInGameController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (IsValid(PlayerController->DefaultMappingContext))
			{
				Subsystem->AddMappingContext(PlayerController->DefaultMappingContext, 0);
			}
			else
			{
				Debug::PrintError(TEXT("ADomiCharacter::NotifyControllerChanged : Invalid MappingContext."));
			}
		}
	}
}

void ADomiCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	InputComponent = PlayerInputComponent;
	BindInputFunctions();

	// if (ControlComponent->bIsComponentReady)
	// {
	// 	BindInputFunctions();
	// }
	// ControlComponent->OnComponentReady.BindUObject(this, &ADomiCharacter::BindInputFunctions);
}

void ADomiCharacter::BindInputFunctions()
{
	if (auto PlayerController = Cast<AInGameController>(Controller))
	{
		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// Moving
			if (IsValid(PlayerController->MoveAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Triggered,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::Move);
			}

			// Looking
			if (IsValid(PlayerController->LookAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->LookAction, ETriggerEvent::Triggered,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::Look);
			}

			// Dash
			if (IsValid(PlayerController->DashAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->DashAction, ETriggerEvent::Started,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::Dash);
			}

			// Dash
			if (IsValid(PlayerController->SprintAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Started,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::Sprint);
			}

			// Parry
			if (IsValid(PlayerController->ParryAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->ParryAction, ETriggerEvent::Started,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::Parry);
			}

			// BaseAttack
			if (IsValid(PlayerController->BaseAttackAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->BaseAttackAction, ETriggerEvent::Started,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::BaseAttack);
			}

			// WeaponSkill
			if (IsValid(PlayerController->WeaponSkillAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->WeaponSkillAction, ETriggerEvent::Started,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::WeaponSkill);
			}

			// MagicSkill
			if (IsValid(PlayerController->MagicSkillAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->MagicSkillAction, ETriggerEvent::Started,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::MagicSkill);
			}

			// Interact
			if (IsValid(PlayerController->InteractAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->InteractAction, ETriggerEvent::Started,
				                                   ControlComponent.Get(),
				                                   &UPlayerControlComponent::Interact);
			}

			// RockOn
			if (IsValid(PlayerController->RockOnAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->RockOnAction, ETriggerEvent::Started,
												   ControlComponent.Get(),
												   &UPlayerControlComponent::RockOn);
			}
		}
		else
		{
			Debug::PrintError(TEXT("ADomiCharacter::BindInputFunctions : Invalid InputComponent."));
		}
	}
}

FGameplayTagContainer ADomiCharacter::GetActiveControlEffectTags()
{
	if (!IsValid(ControlComponent))
	{
		Debug::PrintError(TEXT("ADomiCharacter::GetActiveControlEffectTags : ControlComponent is not valid"));
		return FGameplayTagContainer();
	}

	return ControlComponent->GetActiveControlEffectTags();
}

FGameplayTagContainer ADomiCharacter::GetActiveStatusEffectTags()
{
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("ADomiCharacter::GetActiveStatusEffectTags : StatusComponent is not valid"));
		return FGameplayTagContainer();
	}

	return StatusComponent->GetActiveStatusEffectTags();
}

void ADomiCharacter::InitializeStatusComponent()
{
	FStatusComponentInitializeData InitializeData;

	// Initializing Data for BaseStats
	InitializeData.StatDatas.Add({StatTags::LIFE, 0.f});
	InitializeData.StatDatas.Add({StatTags::STR, 0.f});
	InitializeData.StatDatas.Add({StatTags::DEX, 0.f});

	// Initializing Data for BattleStats
	InitializeData.StatDatas.Add({StatTags::MaxHealth, 100.f});
	InitializeData.StatDatas.Add({StatTags::AttackPower, 10.f});
	InitializeData.StatDatas.Add({StatTags::Defense, 100.f});
	InitializeData.StatDatas.Add({StatTags::MoveSpeed, 1.f});

	// Initializing Data for VariableStats
	InitializeData.StatDatas.Add({StatTags::Health, 100.f});
	InitializeData.StatDatas.Add({StatTags::Stamina, 100.f});

	// Initializing Data for BattleStatMultipliers
	InitializeData.StatMultiplierDatas.Add({StatTags::MaxHealth, 1.f});
	InitializeData.StatDatas.Add({ StatTags::MaxStamina, 100.f });
	InitializeData.StatMultiplierDatas.Add({StatTags::AttackPower, 1.f});
	InitializeData.StatMultiplierDatas.Add({StatTags::Defense, 1.f});
	InitializeData.StatMultiplierDatas.Add({StatTags::MoveSpeed, 1.f});

	// Initializing Data for StatusEffectClasses
	InitializeData.EffectClassDatas.Add({EffectTags::Running, UPlayerRunningEffect::StaticClass()});
	InitializeData.EffectClassDatas.Add({EffectTags::Poison, UPoisonEffect::StaticClass()});
	InitializeData.EffectClassDatas.Add({EffectTags::AttackDown, UAttackDownEffect::StaticClass()});

	StatusComponent->OnDeath.BindUObject(this, &ADomiCharacter::OnDeath);
	
	Debug::Print(TEXT("ADomiCharacter::InitializeStatusComponent : Call."));
	StatusComponent->InitializeStatusComponent(InitializeData);
}

void ADomiCharacter::OnDeath()
{
	ControlComponent->ActivateControlEffect(EffectTags::Death);

	// 델리게이트로?
	ABaseGameMode* GameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(GameMode))
	{
		GameMode->OnPlayerDeath();
	}
}

void ADomiCharacter::InitializeSkillComponent()
{
	FSkillComponentInitializeData InitializeData;

	// Initializing Data for SkillGroups
	// 추후에 데이터 에셋화 혹은 테이터 테이블화
	
	FSkillGroupInitializeData BaseSkillGroupInitializeData;
	BaseSkillGroupInitializeData.SkillGroupTag = SkillGroupTags::BaseAttack;
	BaseSkillGroupInitializeData.SkillGroupData.Add(UBaseAttack::StaticClass());
	InitializeData.SkillGroupInitializeDatas.Add(BaseSkillGroupInitializeData);

	if (IsValid(SkillComponent))
	{
		SkillComponent->InitializeSkillComponent(InitializeData);
	}
}

void ADomiCharacter::ExecuteSkill(FGameplayTag SkillGroupTag)
{
	if (IsValid(SkillComponent))
	{
		SkillComponent->ExecuteSkill(SkillGroupTag);
	}
	else
	{
		Debug::PrintError(TEXT("ADomiCharacter::ExecuteSkill : Invalid SkillComponent."));
	}
}

void ADomiCharacter::OnAttacked_Implementation(const FAttackData& AttackData)
{
	IDamagable::OnAttacked_Implementation(AttackData);

	if (bIsInvincible)
	{
		return;
	}

	if (!IsValid(ControlComponent))
	{
		Debug::PrintError(TEXT("ADomiCharacter::OnAttacked : ControlComponent is not valid"));
		return;
	}

	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("ADomiCharacter::OnAttacked : StatusComponent is not valid"));
		return;
	}

	float CurrentHealth = StatusComponent->GetStat(StatTags::Health);
	StatusComponent->SetHealth(CurrentHealth - AttackData.Damage);

	LaunchCharacter(AttackData.LaunchVector, true, true);

	for (FEffectData EffectData : AttackData.Effects)
	{
		auto [EffectTag, Magnitude, Duration] = EffectData;

		if (EffectTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Effect.Control"))))
		{
			ControlComponent->ActivateControlEffect(EffectTag, Duration);
		}
		else
		{
			StatusComponent->ActivateStatusEffect(EffectTag, Magnitude, Duration);
		}
	}
}

void ADomiCharacter::ShowControlEffectTags_Implementation()
{
	IEffectReceivable::ShowControlEffectTags_Implementation();

	for (auto Tag : GetActiveControlEffectTags().GetGameplayTagArray())
	{
		Debug::Print(Tag.ToString());
	}
}

void ADomiCharacter::ShowStatusEffectTags_Implementation()
{
	IEffectReceivable::ShowStatusEffectTags_Implementation();

	for (auto Tag : GetActiveStatusEffectTags().GetGameplayTagArray())
	{
		Debug::Print(Tag.ToString());
	}
}

FVector ADomiCharacter::GetDashDirection() const
{
	FVector Input = GetLastMovementInputVector();

	if (Input.IsNearlyZero())
	{
		return -GetActorForwardVector();
	}
	return Input.GetSafeNormal();
}

bool ADomiCharacter::HasEnoughStamina() const
{
	if (!StatusComponent) return false;
	return StatusComponent->GetStat(StatTags::Stamina) >= DashStaminaCost;
}

bool ADomiCharacter::StartDash()
{
	if (!StatusComponent || !ControlComponent) return false;
	if (!HasEnoughStamina())
	{
		UE_LOG(LogTemp, Warning, TEXT("StartDash failed: Not enough stamina."));

		return false;
	}
	StatusComponent->ConsumeStamina(DashStaminaCost);
	ControlComponent->ActivateControlEffect(EffectTags::UsingDash, DashDuration);
	SetInvincible(true);

	return true;
}

void ADomiCharacter::EndDash()
{
	SetInvincible(false);
}

void ADomiCharacter::SetInvincible(bool bInvincible)
{
	bIsInvincible = bInvincible;
}