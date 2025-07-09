// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferedMagicSkill.h"

void UBufferedMagicSkill::Operate()
{
	check(ControlComponent);

	ControlComponent->GetWorld()->GetTimerManager().ClearTimer(ValidTimerHandle);
	ControlComponent->MagicSkill();
}
