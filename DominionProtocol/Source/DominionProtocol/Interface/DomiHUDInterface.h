// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DomiHUDInterface.generated.h"

UINTERFACE(MinimalAPI)
class UDomiHUDInterface : public UInterface
{
	GENERATED_BODY()
};


class DOMINIONPROTOCOL_API IDomiHUDInterface
{
	GENERATED_BODY()

public:
	// Initialize & Binding
	virtual void SetupHUDWidget(class UDomiHUDWidget* InDomiHUDWidget) = 0;
};
