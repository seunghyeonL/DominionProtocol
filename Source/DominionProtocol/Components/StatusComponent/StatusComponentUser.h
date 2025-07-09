﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatusComponentUser.generated.h"

class UStatusComponent;
class UStatusComponentBase;
struct FGameplayTagContainer;

// This class does not need to be modified.
UINTERFACE()
class UStatusComponentUser : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOMINIONPROTOCOL_API IStatusComponentUser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UStatusComponent* GetStatusComponent() const = 0;
	virtual void InitializeStatusComponent() = 0;
	virtual FGameplayTagContainer& GetActiveStatusEffectTags() = 0;
};
