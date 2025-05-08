// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusComponent.h"

#include "Components/StatusComponent/StatusComponentUser.h"
#include "Components/StatusComponent/StatusEffects/StatusEffectBase.h"


// Sets default values for this component's properties
UPlayerStatusComponent::UPlayerStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initialize BaseStats
	BaseStats.Add(StatTags::LIFE, 0.f);
	BaseStats.Add(StatTags::STR, 0.f);
	BaseStats.Add(StatTags::DEX, 0.f);

	// Initalize BattleStats
	BattleStats.Add(StatTags::MaxHealth, 100.f);
	BattleStats.Add(StatTags::MaxStamina, 100.f);
	BattleStats.Add(StatTags::AttackPower, 100.f);
	BattleStats.Add(StatTags::Defense, 100.f);
	BattleStats.Add(StatTags::MoveSpeed, 1.f);

	Health = BattleStats[StatTags::MaxHealth];
	Stamina = BattleStats[StatTags::MaxStamina];

	// Initialize BattleStatMultipliers
	BattleStatMultipliers.Add(StatTags::MaxHealth, 1.f);
	BattleStatMultipliers.Add(StatTags::MaxStamina, 1.f);
	BattleStatMultipliers.Add(StatTags::AttackPower, 1.f);
	BattleStatMultipliers.Add(StatTags::Defense, 1.f);
	BattleStatMultipliers.Add(StatTags::MoveSpeed, 1.f);
}


// Called every frame
void UPlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}


