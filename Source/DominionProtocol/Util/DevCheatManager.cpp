// Fill out your copyright notice in the Description page of Project Settings.


#include "DevCheatManager.h"
#include "Util/CheatBPLib.h"

void UDevCheatManager::Save()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		UCheatBPLib::Save(World);
	}
}
