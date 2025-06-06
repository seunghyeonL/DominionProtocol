// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/EffectData/EffectUIData.h"
#include "UObject/Interface.h"
#include "Util/GameTagList.h"
#include "EffectUser.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UEffectUser : public UInterface
{
	GENERATED_BODY()
};

class DOMINIONPROTOCOL_API IEffectUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTagContainer GetAllActivateEffectDatas() const;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SendEffectUIDatas() const;
};
