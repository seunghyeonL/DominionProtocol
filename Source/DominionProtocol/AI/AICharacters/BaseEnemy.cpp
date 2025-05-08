// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseEnemy::OnDeath()
{
}

void ABaseEnemy::OnGroggy()
{
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::LookAtTarget(const AActor* TargetActor)
{
}

void ABaseEnemy::OnAttacked_Implementation(const FAttackData& AttackData)
{
	IDamagable::OnAttacked_Implementation(AttackData);
}

void ABaseEnemy::ExecutePattern(FGameplayTag SkillGroupTag)
{
}

FGameplayTagContainer ABaseEnemy::GetActiveControlEffectTags_Implementation()
{
	return IEffectReceivable::GetActiveControlEffectTags_Implementation();
}

FGameplayTagContainer ABaseEnemy::GetActiveStatusEffectTags_Implementation()
{
	return IEffectReceivable::GetActiveStatusEffectTags_Implementation();
}

void ABaseEnemy::InitializeStatusComponent()
{
}

