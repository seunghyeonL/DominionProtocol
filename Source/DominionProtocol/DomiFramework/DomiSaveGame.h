// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EnumAndStruct/FInstanceData.h"
#include "EnumAndStruct/FItemSubsystemData.h"
#include "EnumAndStruct/FSoundSubsystemData.h"
#include "EnumAndStruct/FWorldInstanceSubsystemData.h"
#include "DomiSaveGame.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FInstanceData InstanceData;

	UPROPERTY()
	FItemSubsystemData ItemSubsystemData;

	UPROPERTY()
	FWorldInstanceSubsystemData WorldInstanceSubsystemData;
};
