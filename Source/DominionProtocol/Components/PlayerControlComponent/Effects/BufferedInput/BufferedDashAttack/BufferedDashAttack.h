// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/BufferedInput/BaseBufferedInput.h"
#include "BufferedDashAttack.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBufferedDashAttack : public UBaseBufferedInput
{
	GENERATED_BODY()

public:
	virtual void Operate() override;
};
