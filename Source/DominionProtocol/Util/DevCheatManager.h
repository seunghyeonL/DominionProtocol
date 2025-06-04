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
	void ToggleDebugLines();

	UFUNCTION(Exec)
	void ToggleMnhDebug();

	UFUNCTION(Exec)
	void ToggleAllDebug();

	UFUNCTION(Exec)
	void InfiniteStamina();

	UFUNCTION(Exec)
	void MoveToCrack(FString TargetLevelName, int32 TargetCrackIndex);

	UFUNCTION(Exec)
	void StoryState(EGameStoryState NewStoryState);

	UFUNCTION(Exec)
	void Suicide();

	UFUNCTION(Exec)
	void AddAllItems();

	UFUNCTION(Exec)
	void ActivateEffect(FName EffectTagName, float Magnitude, float Duration);

	UFUNCTION(Exec)
	void ToggleFog();
};
