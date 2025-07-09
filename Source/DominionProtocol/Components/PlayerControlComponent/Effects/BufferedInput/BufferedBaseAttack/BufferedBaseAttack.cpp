// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferedBaseAttack.h"

#include "Util/DebugHelper.h"

void UBufferedBaseAttack::Operate()
{
	check(ControlComponent);

	ControlComponent->GetWorld()->GetTimerManager().ClearTimer(ValidTimerHandle);
	ControlComponent->BaseAttack();
}
