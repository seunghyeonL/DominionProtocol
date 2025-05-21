// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateBase.h"
#include "Components/StatusComponent/StatusComponent.h"

void UAIStateBase::Activate()
{
	Super::Activate();

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!StatusComponent)
	{
		Debug::PrintError(TEXT("UAIStateBase::Activate : Invalid StatusComponent"));
		return;
	}

	// 현재 상태가 존재하면 비활성화
	if (UAIStateBase* CurrentAIState = StatusComponent->GetAIState())
	{
		CurrentAIState->Deactivate();
	}

	StatusComponent->SetAIState(this);
	StatusComponent->GetActiveStatusEffectTags().AddTag(StateTag);
	bIsActive = true;


	UE_LOG(LogTemp, Warning, TEXT("AI State Activated: %s"), *StateTag.ToString());
}

void UAIStateBase::Activate(float Duration)
{
	// Super::Activate(Duration);
	Debug::PrintError(TEXT("UAIStateBase::Activate : Use Activate function with no Duration parameter."));
}

void UAIStateBase::Deactivate()
{
	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!StatusComponent)
	{
		Debug::PrintError(TEXT("UAIStateBase::Deactivate : Invalid StatusComponent"));
		return;
	}

	StatusComponent->GetActiveStatusEffectTags().RemoveTag(StateTag);
	bIsActive = false;

	UE_LOG(LogTemp, Warning, TEXT("AI State Deactivated: %s"), *StateTag.ToString());
}
