// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ControlComponentUser.generated.h"

class UPlayerControlComponent;
struct FGameplayTagContainer;

// This class does not need to be modified.
UINTERFACE(BlueprintType, Blueprintable)
class UControlComponentUser : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOMINIONPROTOCOL_API IControlComponentUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UPlayerControlComponent* GetPlayerControlComponent() const = 0;
	virtual FGameplayTagContainer GetActiveControlEffectTags() = 0;
};
