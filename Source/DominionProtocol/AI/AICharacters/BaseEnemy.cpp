// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/StatusComponent/StatusComponentInitializeData.h"
#include "Components/StatusComponent/StatusEffects/AIDeathEffect/AIDeathEffect.h"
#include "Components/StatusComponent/StatusEffects/AIStiffnessEffect/AIStiffnessEffect.h"
#include "Components/StatusComponent/StatusEffects/AIStunEffect/AIStunEffect.h"
#include "Components/StatusComponent/StatusEffects/AttackDownEffect/AttackDownEffect.h"
#include "Components/StatusComponent/StatusEffects/PoisonEffect/PoisonEffect.h"
#include "UI/DomiWidgetComponent.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// BattleComponents
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));

	// UI Section
	HPWidgetComponent = CreateDefaultSubobject<UDomiWidgetComponent>(TEXT("DomiWidgetComponent"));
	HPWidgetComponent->SetupAttachment(GetMesh());
	HPWidgetComponent->SetRelativeLocation(FVector(0, 0, 180));
	static ConstructorHelpers::FClassFinder<UUserWidget> HPBarWidgetRef (TEXT("/Game/Blueprints/UI/WBP_DomiMonster3DWidget.WBP_DomiMonster3DWidget_C"));
	if (HPBarWidgetRef.Class)
	{
		HPWidgetComponent->SetWidgetClass(HPBarWidgetRef.Class);
		HPWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
		HPWidgetComponent->SetDrawSize(FVector2D(100.f, 15.f));
		HPWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemy::OnDeath()
{
	StatusComponent->ActivateStatusEffect(EffectTags::Death, 0);

	// Ignore Collision with Pawn
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
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

FGameplayTagContainer ABaseEnemy::GetActiveStatusEffectTags()
{
	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("ADomiCharacter::GetActiveStatusEffectTags : StatusComponent is not valid"));
		return FGameplayTagContainer();
	}

	return StatusComponent->GetActiveStatusEffectTags();
}

void ABaseEnemy::OnAttacked_Implementation(const FAttackData& AttackData)
{
	IDamagable::OnAttacked_Implementation(AttackData);

	if (!IsValid(StatusComponent))
	{
		Debug::PrintError(TEXT("ABaseEnemy::OnAttacked_Implementation : StatusComponent is not valid"));
		return;
	}

	float CurrentHealth = StatusComponent->GetStat(StatTags::Health);
	StatusComponent->SetHealth(CurrentHealth - AttackData.Damage);

	LaunchCharacter(AttackData.LaunchVector, true, true);

	for (FEffectData EffectData : AttackData.Effects)
	{
		auto [EffectTag, Magnitude, Duration] = EffectData;
		
		StatusComponent->ActivateStatusEffectWithDuration(EffectTag, Magnitude, Duration);
	}
}

void ABaseEnemy::ExecutePattern(FGameplayTag SkillGroupTag)
{
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
	FStatusComponentInitializeData InitializeData;

	// Initializing Data for BattleStats
	InitializeData.StatDatas.Add({StatTags::MaxHealth, 100.f});
	InitializeData.StatDatas.Add({StatTags::AttackPower, 100.f});
	InitializeData.StatDatas.Add({StatTags::Defense, 100.f});
	InitializeData.StatDatas.Add({StatTags::MoveSpeed, 1.f});

	// Initializing Data for VariableStats
	InitializeData.StatDatas.Add({StatTags::Health, 100.f});

	// Initializing Data for BattleStatMultipliers
	InitializeData.StatMultiplierDatas.Add({StatTags::MaxHealth, 1.f});
	InitializeData.StatMultiplierDatas.Add({StatTags::MaxStamina, 1.f});
	InitializeData.StatMultiplierDatas.Add({StatTags::AttackPower, 1.f});
	InitializeData.StatMultiplierDatas.Add({StatTags::Defense, 1.f});
	InitializeData.StatMultiplierDatas.Add({StatTags::MoveSpeed, 1.f});

	// Initializing Data for StatusEffectClasses
	InitializeData.EffectClassDatas.Add({EffectTags::Stun, UAIStunEffect::StaticClass()});
	InitializeData.EffectClassDatas.Add({EffectTags::Stiffness, UAIStiffnessEffect::StaticClass()});
	InitializeData.EffectClassDatas.Add({EffectTags::Death, UAIDeathEffect::StaticClass()});
	InitializeData.EffectClassDatas.Add({EffectTags::Poison, UPoisonEffect::StaticClass()});
	InitializeData.EffectClassDatas.Add({EffectTags::AttackDown, UAttackDownEffect::StaticClass()});

	Debug::Print(TEXT("ABaseEnemy::InitializeStatusComponent : Call."));
	StatusComponent->InitializeStatusComponent(InitializeData);

	//현재 상태 바꾸는 로직 필요
}


