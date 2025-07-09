// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PoolableObjectInterface.generated.h"

UINTERFACE()
class UPoolableObjectInterface : public UInterface
{
	GENERATED_BODY()
};

class DOMINIONPROTOCOL_API IPoolableObjectInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnObjectSpawn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnObjectReturn();
};
