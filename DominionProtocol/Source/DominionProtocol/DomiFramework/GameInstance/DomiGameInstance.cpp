// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiGameInstance.h"

const int32 UDomiGameInstance::NumBosses = 1;

UDomiGameInstance::UDomiGameInstance()
{
	IsBossDeadArray.Init(false, NumBosses);
}

void UDomiGameInstance::SetIsBossDead(int32 BossIndex)
{
	check(IsBossDeadArray.IsValidIndex(BossIndex));
	IsBossDeadArray[BossIndex] = true;
}

bool UDomiGameInstance::GetIsBossDead(int32 BossIndex)
{
	check(IsBossDeadArray.IsValidIndex(BossIndex));
	return IsBossDeadArray[BossIndex];
}
