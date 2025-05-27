// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "DevCheatManager.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDevCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
	UFUNCTION(Exec)
	void Save();

	UFUNCTION(Exec)
	void InfiniteStamina();

	UFUNCTION(Exec)
	void MoveToCrack(FString TargetLevelName, int32 TargetCrackIndex);

	UFUNCTION(Exec)
	void StoryState(EGameStoryState NewStoryState);

	UFUNCTION(Exec)
	void Suicide();
};
