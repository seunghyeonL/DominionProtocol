// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AIComponent/AIStateComponent.h"
#include "AIState/AIStateBase.h"

// Sets default values for this component's properties
UAIStateComponent::UAIStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UAIStateComponent::SetAIStateByTag(const FGameplayTag& StateTag)
{
	if (CurrentState && CurrentState->GetStateTag() == StateTag)
		return;

	// 현재 상태 비활성화
	if (CurrentState)
	{
		CurrentState->Deactivate();
	}

	// 없으면 생성해서 캐싱
	if (!StateMap.Contains(StateTag))
	{
		UAIStateBase* NewState = NewObject<UAIStateBase>(this);
		NewState->StateTag = StateTag;
		StateMap.Add(StateTag, NewState);
	}

	// 새 상태로 전환
	CurrentState = StateMap[StateTag];
	CurrentState->Activate();

	// 알림 브로드캐스트
	OnAIStateChanged.Broadcast(StateTag);
}

FGameplayTag UAIStateComponent::GetCurrentStateTag() const
{
	if (CurrentState)
	{
		return CurrentState->GetStateTag();
	}
	return FGameplayTag();
}