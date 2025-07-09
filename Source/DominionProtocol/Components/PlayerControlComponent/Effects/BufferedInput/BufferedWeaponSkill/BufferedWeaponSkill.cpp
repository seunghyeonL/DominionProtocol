// Fill out your copyright notice in the Description page of Project Settings.


#include "BufferedWeaponSkill.h"

void UBufferedWeaponSkill::Operate()
{
	check(ControlComponent);

	ControlComponent->GetWorld()->GetTimerManager().ClearTimer(ValidTimerHandle);
	ControlComponent->WeaponSkill();
}
