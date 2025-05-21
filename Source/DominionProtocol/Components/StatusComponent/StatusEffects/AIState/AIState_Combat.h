// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusEffects/AIState/AIStateBase.h"
#include "AIState_Combat.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAIState_Combat : public UAIStateBase
{
	GENERATED_BODY()
public:
	UAIState_Combat()
	{
		StateTag = EffectTags::Combat;
	}
};
