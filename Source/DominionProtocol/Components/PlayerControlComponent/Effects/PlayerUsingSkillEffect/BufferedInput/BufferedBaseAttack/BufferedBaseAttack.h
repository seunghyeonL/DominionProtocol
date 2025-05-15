// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/PlayerUsingSkillEffect/BufferedInput/BaseBufferedInput.h"
#include "BufferedBaseAttack.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBufferedBaseAttack : public UBaseBufferedInput
{
	GENERATED_BODY()

public:
	virtual void Operate() override;
};
