// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateBase.h"

#include "Components/StatusComponent/StatusComponent.h"

void UAIStateBase::Activate(float Magnitude)
{
	Super::Activate(Magnitude);

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (auto CurrentAIState = StatusComponent->GetAIState())
	{
		CurrentAIState->Deactivate();
		StatusComponent->SetAIState(this);
	}
	else
	{
		Debug::PrintError(TEXT("UAIStateBase::Activate : Invalid current AIState."));
	}
}

void UAIStateBase::Activate(float Magnitude, float Duration)
{
	// Super::Activate(Magnitude, Duration);
	Debug::PrintError(TEXT("UAIStateBase::Activate : Use Activate function with no Duration parameter."));
}

void UAIStateBase::Deactivate()
{
	Super::Deactivate();
}
