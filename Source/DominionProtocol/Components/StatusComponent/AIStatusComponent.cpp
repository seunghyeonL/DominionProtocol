// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusComponent/AIStatusComponent.h"

// Sets default values for this component's properties
UAIStatusComponent::UAIStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	ActiveTags.AddTag(EffectTags::Idle);
}


void UAIStatusComponent::AddTag(const FGameplayTag& Tag)
{
	ActiveTags.AddTag(Tag);
}

void UAIStatusComponent::RemoveTag(const FGameplayTag& Tag)
{
	ActiveTags.RemoveTag(Tag);
}

void UAIStatusComponent::SetExclusiveState(const FGameplayTag& NewState)
{
	// 내부 고정 목록 사용
	static const TArray<FGameplayTag> AllStates = {
		EffectTags::Idle,
		EffectTags::Suspicious,
		EffectTags::Alert,
		EffectTags::Combat,
		EffectTags::Patrol,
		EffectTags::Return
	};

	ActiveTags.RemoveTags(FGameplayTagContainer::CreateFromArray(AllStates));
	ActiveTags.AddTag(NewState);
}