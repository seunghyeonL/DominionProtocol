// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/FSoundSubsystemData.h"
#include "GameFramework/SaveGame.h"
#include "DomiSaveSettings.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiSaveSettings : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FSoundSubsystemData SoundSubsystemData;
};
