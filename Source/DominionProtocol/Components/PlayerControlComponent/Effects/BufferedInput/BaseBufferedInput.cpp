// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBufferedInput.h"

UBaseBufferedInput::UBaseBufferedInput()
{
	bIsExpired = false;
	ExpireDuration = 0.3f;
}

void UBaseBufferedInput::SetTimer()
{
	TWeakObjectPtr<ThisClass> WeakThis{this};
	ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	if (IsValid(ControlComponent))
	{
		ControlComponent->GetWorld()->GetTimerManager().SetTimer(
			ValidTimerHandle,
			[WeakThis]()
			{
				if (!WeakThis.IsValid())
				{
					return;
				}

				WeakThis->bIsExpired = true;
			},
			ExpireDuration,
			false
			);
	}
}

void UBaseBufferedInput::Operate()
{
	// 하위에서 구현
}
