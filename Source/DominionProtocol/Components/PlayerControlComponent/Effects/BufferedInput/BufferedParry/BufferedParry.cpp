// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferedParry.h"

void UBufferedParry::Operate()
{
	check(ControlComponent);

	ControlComponent->GetWorld()->GetTimerManager().ClearTimer(ValidTimerHandle);
	ControlComponent->Parry();
}
