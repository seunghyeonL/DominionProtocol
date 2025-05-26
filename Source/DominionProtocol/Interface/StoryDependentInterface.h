// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "StoryDependentInterface.generated.h"

UINTERFACE(BlueprintType, Blueprintable)
class UStoryDependentInterface : public UInterface
{
	GENERATED_BODY()
};

class DOMINIONPROTOCOL_API IStoryDependentInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Story")
	void OnStoryStateUpdated(EGameStoryState NewState);
};
