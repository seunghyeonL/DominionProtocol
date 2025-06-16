// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/BufferedInput/BaseBufferedInput.h"
#include "BufferedParry.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBufferedParry : public UBaseBufferedInput
{
	GENERATED_BODY()

public:
	virtual void Operate() override;
};
