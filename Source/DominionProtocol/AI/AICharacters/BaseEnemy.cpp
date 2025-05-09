// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// BattleComponents
	StatusComponent = CreateDefaultSubobject<UStatusComponent>(TEXT("StatusComponent"));
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

void ABaseEnemy::LookAtTarget(const AActor* TargetActor)
{
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
}

void ABaseEnemy::ExecutePattern(FGameplayTag SkillGroupTag)
{
}

void ABaseEnemy::ShowControlEffectTags_Implementation()
{
	IEffectReceivable::ShowControlEffectTags_Implementation();

	// Not Use ControlComponent
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
	
}

