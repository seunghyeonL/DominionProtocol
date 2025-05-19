// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusEffects/AIState/AIStateBase.h"
#include "AIState_Return.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAIState_Return : public UAIStateBase
{
	GENERATED_BODY()
public:
	UAIState_Return()
	{
		StateTag = EffectTags::Return;
	}
};
