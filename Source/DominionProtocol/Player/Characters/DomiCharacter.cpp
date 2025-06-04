// Copyright Epic Games, Inc. All Rights Reserved.

#include "DomiCharacter.h"
#include "Util/DebugHelper.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "NavigationInvokerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "../InGameController.h"
#include "Util/BattleDataTypes.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "Components/StatusComponent/StatusComponentInitializeData.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"

#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameState/BaseGameState.h"

#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"


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
	CameraBoom->bEnableCameraLag = true; // Camera follow pawn smoothly
	CameraBoom->CameraLagSpeed = 5.0f;
	

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Battle Components
	ControlComponent = CreateDefaultSubobject<UPlayerControlComponent>(TEXT("ControlComponent"));
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));
	ItemComponent = CreateDefaultSubobject<UItemComponent>(TEXT("ItemComponent"));
	AttackTraceComponent = CreateDefaultSubobject<UMnhTracerComponent>(TEXT("AttackTraceComponent"));

	// NavigationInvoker Component
	NavigationInvokerComponent = CreateDefaultSubobject<UNavigationInvokerComponent>(TEXT("NavigationInvokerComponent"));

	NavigationInvokerComponent->SetGenerationRadii(6000.f, 10000.f);

	// TraceBox
	WeaponTraceBox = CreateDefaultSubobject<UMnhBoxComponent>(TEXT("WeaponTraceBox"));

	TempWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempWeapon"));
	TempWeapon->SetupAttachment(GetMesh(), FName("TempWeaponSocket"));

	WeaponTraceBox->SetupAttachment(TempWeapon);

	FMnhTracerConfig TracerConfig;

	TracerConfig.TracerTag = ItemTags::BasicWeapon;
	TracerConfig.DrawDebugType = EDrawDebugTrace::ForDuration;
	TracerConfig.DebugDrawTime = 2.f;
	TracerConfig.TraceSettings.TraceChannel = ECC_Pawn;

	AttackTraceComponent->TracerConfigs.Add(TracerConfig);

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(ItemTags::BasicWeapon);

	AttackTraceComponent->InitializeTracers(TagContainer, WeaponTraceBox);

	// InvincibilityTags Setting
	InvincibilityTags.AddTag(EffectTags::UsingDash);
	InvincibilityTags.AddTag(EffectTags::Death);
	InvincibilityTags.AddTag(EffectTags::UsingZoneya);
	
	// Set PawnTag
	PawnTag = PawnTags::Player;

	PlayerName = TEXT("DefaultPlayer");

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

AActor* ADomiCharacter::GetCurrentInteractableActor() const
{
	if (InteractableActor == nullptr)
	{
		Debug::Print("ADomiCharacter::GetCurrentInteractableObject : InteractableObject == nullptr");
		return nullptr;
	}
	else
	{
		return InteractableActor;
	}
}

void ADomiCharacter::AddInteractableActor(AActor* AddInteractableActor)
{
	if (!InteractableActorSet.Contains(AddInteractableActor))
	{
		InteractableActorSet.Add(AddInteractableActor);
		OnAddInteractableActor.Broadcast(InteractableActorSet);	
	}
}

void ADomiCharacter::RemoveInteractableActor(AActor* RemoveInteractableActor)
{
	if (InteractableActorSet.Contains(RemoveInteractableActor))
	{
		InteractableActorSet.Remove(RemoveInteractableActor);
		OnRemoveInteractableActor.Broadcast(InteractableActorSet);	
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

				EnhancedInputComponent->BindAction(PlayerController->MoveAction, ETriggerEvent::Completed,
												   ControlComponent.Get(),
												   &UPlayerControlComponent::ResetLastMovementVector);
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
				                                   &UPlayerControlComponent::SprintStart);
				EnhancedInputComponent->BindAction(PlayerController->SprintAction, ETriggerEvent::Completed,
												   ControlComponent.Get(),
												   &UPlayerControlComponent::SprintEnd);
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

			// LockOn
			if (IsValid(PlayerController->LockOnAction))
			{
				EnhancedInputComponent->BindAction(PlayerController->LockOnAction, ETriggerEvent::Started,
												   ControlComponent.Get(),
												   &UPlayerControlComponent::LockOn);
			}

			// Consume Item (Slot 0)
			if (IsValid(PlayerController->ConsumeItemAction_1))
			{
				EnhancedInputComponent->BindAction(PlayerController->ConsumeItemAction_1, ETriggerEvent::Started,
													ControlComponent.Get(),
													&UPlayerControlComponent::ConsumeItemAction_1);
			}

			// Consume Item (Slot 1)
			if (IsValid(PlayerController->ConsumeItemAction_2))
			{
				EnhancedInputComponent->BindAction(PlayerController->ConsumeItemAction_2, ETriggerEvent::Started,
					ControlComponent.Get(),
					&UPlayerControlComponent::ConsumeItemAction_2);
			}

			// Consume Item (Slot 2)
			if (IsValid(PlayerController->ConsumeItemAction_3))
			{
				EnhancedInputComponent->BindAction(PlayerController->ConsumeItemAction_3, ETriggerEvent::Started,
					ControlComponent.Get(),
					&UPlayerControlComponent::ConsumeItemAction_3);
			}

			// Swap Weapon
			if (IsValid(PlayerController->SwapWeapon))
			{
				EnhancedInputComponent->BindAction(PlayerController->SwapWeapon, ETriggerEvent::Started,
					ControlComponent.Get(),
					&UPlayerControlComponent::SwapWeapon);
			}

			if (IsValid(PlayerController->InteractionScroll))
			{
				EnhancedInputComponent->BindAction(PlayerController->InteractionScroll, ETriggerEvent::Triggered,
					ControlComponent.Get(),
					&UPlayerControlComponent::InteractionScroll);
			}

			if (IsValid(PlayerController->SwitchShowAndHideInventoryWidget))
			{
				EnhancedInputComponent->BindAction(PlayerController->SwitchShowAndHideInventoryWidget, ETriggerEvent::Started,
					ControlComponent.Get(),
					&UPlayerControlComponent::SwitchShowAndHideInventory);
			}
		}
		else
		{
			Debug::PrintError(TEXT("ADomiCharacter::BindInputFunctions : Invalid InputComponent."));
		}
	}
}

FGameplayTagContainer& ADomiCharacter::GetActiveControlEffectTags()
{
	check(ControlComponent);

	return ControlComponent->GetActiveControlEffectTags();
}

void ADomiCharacter::SkillStart(FGameplayTag ControlEffectTag)
{
	check(ControlComponent);
	
	ControlComponent->ActivateControlEffect(ControlEffectTag);
}

void ADomiCharacter::SkillEnd(FGameplayTag ControlEffectTag)
{
	check(ControlComponent);
	
	ControlComponent->DeactivateControlEffect(ControlEffectTag);
}


FGameplayTagContainer& ADomiCharacter::GetActiveStatusEffectTags()
{
	check(StatusComponent);
	
	return StatusComponent->GetActiveStatusEffectTags();
}

void ADomiCharacter::InitializeStatusComponent()
{
	if (auto World = GetWorld())
	{
		if (auto BaseGameState = World->GetGameState<ABaseGameState>())
		{
			if (FStatusComponentInitializeData* InitializeData = BaseGameState->GetStatusComponentInitializeData(PawnTag))
			{
				StatusComponent->InitializeStatusComponent(*InitializeData);
				StatusComponent->OnDeath.AddUObject(this, &ADomiCharacter::OnDeath);
			}
		}
	}
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

FString ADomiCharacter::GetPawnName_Implementation()
{
	return PlayerName;
}

void ADomiCharacter::InitializeSkillComponent()
{
	if (auto World = GetWorld())
	{
		if (auto BaseGameState = World->GetGameState<ABaseGameState>())
		{
			if (FSkillComponentInitializeData* InitializeData = BaseGameState->GetSkillComponentInitializeData(PawnTag))
			{
				SkillComponent->SetSkills(*InitializeData);
				SkillComponent->OnSkillStart.BindUObject(this, &ADomiCharacter::SkillStart);
				SkillComponent->OnSkillEnd.BindUObject(this, &ADomiCharacter::SkillEnd);
			}
		}
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

	check(ControlComponent);
	check(SkillComponent);
	check(StatusComponent);
	
	auto& ActiveControlEffects = GetActiveControlEffectTags();

	if (ActiveControlEffects.HasAny(InvincibilityTags))
	{
		Debug::Print(TEXT("ADomiCharacter::OnAttacked : Invincible!"));
		return;
	}

	float CurrentHealth = StatusComponent->GetStat(StatTags::Health);
	StatusComponent->SetHealth(CurrentHealth - AttackData.Damage);

	LaunchCharacter(AttackData.LaunchVector, true, true);

	// Activate Effects
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

FGameplayTag ADomiCharacter::GetPawnTag_Implementation()
{
	return PawnTag;
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

void ADomiCharacter::ActivateStatusEffect_Implementation(const FGameplayTag& EffectTag)
{
	IEffectReceivable::ActivateStatusEffect_Implementation(EffectTag);

	check(StatusComponent);

	StatusComponent->ActivateStatusEffect(EffectTag, 0);
}

bool ADomiCharacter::IsParryingCond()
{
	auto ActiveControlEffects = GetActiveControlEffectTags();
	if (ActiveControlEffects.HasTag(EffectTags::UsingParry))
	{
		return true;
	}
	return false;
}

void ADomiCharacter::OnParried()
{
	// 일단 패리당할일 없어서 비워둠
}

void ADomiCharacter::EventInteractionWidgetScroll(const float Value)
{
	OnInteractionWidgetScroll.Broadcast(Value);
}

FGameplayTagContainer ADomiCharacter::GetAllActivateEffects() const
{
	FGameplayTagContainer ActiveEffects;

	ActiveEffects.AppendTags(StatusComponent->GetActiveStatusEffectTags());
	ActiveEffects.AppendTags(ControlComponent->GetActiveControlEffectTags());
	
	return MoveTemp(ActiveEffects);
}
