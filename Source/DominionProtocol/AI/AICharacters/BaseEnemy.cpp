// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "Components/StatusComponent/StatusComponentInitializeData.h"
#include "Components/WidgetComponent/DomiWidgetComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// BattleComponents
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
	SkillComponent = CreateDefaultSubobject<USkillComponent>(TEXT("SkillComponent"));

	// UI Section
	HPWidgetComponent = CreateDefaultSubobject<UDomiWidgetComponent>(TEXT("DomiWidgetComponent"));
	HPWidgetComponent->SetupAttachment(GetMesh());
	HPWidgetComponent->SetRelativeLocation(FVector(0, 0, 190));
	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetRef (TEXT("/Game/Blueprints/UI/WBP_DomiMonster3DWidget.WBP_DomiMonster3DWidget_C"));
	if (HPBarWidgetRef.Class)
	{
		HPWidgetComponent->SetWidgetClass(HPBarWidgetRef.Class);
		HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		HPWidgetComponent->SetDrawSize(FVector2D(80.f, 10.f));
		HPWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// InvincibilityTags Setting
	InvincibilityTags.AddTag(EffectTags::Death);

	// HardCCTags Setting
	HardCCTags.AddTag(EffectTags::Stun);
	HardCCTags.AddTag(EffectTags::Stiffness);
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEnemy::OnDeath()
{
	StatusComponent->ActivateStatusEffect(EffectTags::Death, 0);
}

void ABaseEnemy::OnGroggy()
{
	// for Boss
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FGameplayTagContainer& ABaseEnemy::GetActiveStatusEffectTags()
{
	check(StatusComponent);
	return StatusComponent->GetActiveStatusEffectTags();
}

void ABaseEnemy::InitializeSkillComponent()
{
	if (auto World = GetWorld())
	{
		if (auto BaseGameState = World->GetGameState<ABaseGameState>())
		{
			if (FSkillComponentInitializeData* InitializeData = BaseGameState->GetSkillComponentInitializeData(PawnTag))
			{
				SkillComponent->InitializeSkillComponent(*InitializeData);
				// SkillComponent->OnSkillStart.BindUObject(this, &ADomiCharacter::SkillStart);
				// SkillComponent->OnSkillEnd.BindUObject(this, &ADomiCharacter::SkillEnd);
			}
		}
	}
}

void ABaseEnemy::ExecuteSkill(FGameplayTag SkillGroupTag)
{
	if (IsValid(SkillComponent))
	{
		SkillComponent->ExecuteSkill(SkillGroupTag);
	}
}

void ABaseEnemy::OnAttacked_Implementation(const FAttackData& AttackData)
{
	IDamagable::OnAttacked_Implementation(AttackData);

	check(StatusComponent);
	check(SkillComponent)
	
	auto ActiveStatusEffects = GetActiveStatusEffectTags();
	if (ActiveStatusEffects.HasAny(InvincibilityTags))
	{
		Debug::Print(TEXT("ADomiCharacter::OnAttacked : Invincible!"));
		return;
	}

	if (ActiveStatusEffects.HasAny(HardCCTags))
	{
		SkillComponent->StopSkill();
	}
	
	float CurrentHealth = StatusComponent->GetStat(StatTags::Health);
	StatusComponent->SetHealth(CurrentHealth - AttackData.Damage);

	LaunchCharacter(AttackData.LaunchVector, true, true);

	for (FEffectData EffectData : AttackData.Effects)
	{
		auto [EffectTag, Magnitude, Duration] = EffectData;
		
		StatusComponent->ActivateStatusEffect(EffectTag, Magnitude, Duration);
	}
}

void ABaseEnemy::ShowControlEffectTags_Implementation()
{
	IEffectReceivable::ShowControlEffectTags_Implementation();

	// This Actor Doesn't Use ControlComponent
}

void ABaseEnemy::ShowStatusEffectTags_Implementation()
{
	IEffectReceivable::ShowStatusEffectTags_Implementation();

	for (auto Tag : GetActiveStatusEffectTags().GetGameplayTagArray())
	{
		Debug::Print(Tag.ToString());
	}
}

void ABaseEnemy::InitializeStatusComponent()
{
	if (auto World = GetWorld())
	{
		if (auto BaseGameState = World->GetGameState<ABaseGameState>())
		{
			if (FStatusComponentInitializeData* InitializeData = BaseGameState->GetStatusComponentInitializeData(PawnTag))
			{
				StatusComponent->InitializeStatusComponent(*InitializeData);
				StatusComponent->OnDeath.AddUObject(this, &ABaseEnemy::OnDeath);
			}
		}
	}
}


