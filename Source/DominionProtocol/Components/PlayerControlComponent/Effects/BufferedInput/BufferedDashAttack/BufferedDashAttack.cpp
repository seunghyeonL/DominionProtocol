// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferedDashAttack.h"

void UBufferedDashAttack::Operate()
{
	check(ControlComponent);

	ControlComponent->GetWorld()->GetTimerManager().ClearTimer(ValidTimerHandle);
	ControlComponent->DashAttack();
}
