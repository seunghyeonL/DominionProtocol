// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CheatBPLib.generated.h"

class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API UCheatBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void Save(const UWorld* World);

	static void InfiniteStamina(const TObjectPtr<UStatusComponent> StatusComponent);
};
