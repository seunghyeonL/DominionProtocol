// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferedDash.h"

void UBufferedDash::Operate()
{
	check(ControlComponent);

	ControlComponent->GetWorld()->GetTimerManager().ClearTimer(ValidTimerHandle);
	ControlComponent->Dash();
}
