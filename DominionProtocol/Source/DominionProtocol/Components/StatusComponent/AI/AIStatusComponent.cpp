// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStatusComponent.h"


// Sets default values for this component's properties
UAIStatusComponent::UAIStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	GroggyGauge = 0.f;
	// ...
}


// Called when the game starts
void UAIStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAIStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UAIStatusComponent::GetStat(FGameplayTag StatTag) const
{
	if (StatTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("Stat.Battle"))))
	{
		return BattleStats[StatTag];
	}

	if (StatTag.MatchesTagExact(StatTags::Health))
	{
		return Health;
	}

	if (StatTag.MatchesTagExact(StatTags::GroggyGauge))
	{
		return GroggyGauge;
	}

	Debug::PrintError(TEXT("UAIStatusComponent::GetStat : Finding StatTag is not set."));
	return -1.f;
}

