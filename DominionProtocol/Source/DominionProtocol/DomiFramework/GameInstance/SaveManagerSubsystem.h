// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveManagerSubsystem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API USaveManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void SaveGame(const FString& SlotName, int32 UserIndex = 0);
	bool LoadGame(const FString& SlotName, int32 UserIndex = 0);
};
