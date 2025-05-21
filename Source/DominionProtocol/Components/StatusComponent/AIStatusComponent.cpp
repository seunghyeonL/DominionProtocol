// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StatusComponent/AIStatusComponent.h"
#include "Components/StatusComponent/StatusEffects/AIState/AIStateBase.h"

// Sets default values for this component's properties
UAIStatusComponent::UAIStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAIStatusComponent::SetAIState(UAIStateBase* NewState)
{
	// 상태 해제 (nullptr 전달 시)
	if (!NewState)
	{
		if (CurrentState)
		{
			CurrentState->Deactivate();
			OnAIStateChanged.Broadcast(FGameplayTag());
			CurrentState = nullptr;
		}
		return;
	}

	if (CurrentState == NewState)
		return;

	// 기존 상태 비활성화
	if (CurrentState)
		CurrentState->Deactivate();

	// 새 상태 전환
	CurrentState = NewState;
	CurrentState->Activate();

	// 상태 변경 알림
	OnAIStateChanged.Broadcast(CurrentState->GetStateTag());
}

void UAIStatusComponent::SetAIStateByTag(const FGameplayTag& NewTag)
{
	if (CurrentState && CurrentState->GetStateTag() == NewTag)
		return;

	if (CurrentState)
	{
		CurrentState->Deactivate();
	}

	// 새 상태 생성
	UAIStateBase* NewState = NewObject<UAIStateBase>(this);
	NewState->StateTag = NewTag;

	CurrentState = NewState;
	CurrentStateTag = NewTag;
	NewState->Activate();

	OnAIStateChanged.Broadcast(NewTag);
}

FGameplayTag UAIStatusComponent::GetCurrentStateTag() const
{
	return CurrentState ? CurrentState->GetStateTag() : FGameplayTag();
}