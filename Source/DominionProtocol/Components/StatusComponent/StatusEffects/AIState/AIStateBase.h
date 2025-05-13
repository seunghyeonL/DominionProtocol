// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusEffects/StatusEffectBase.h"
#include "AIStateBase.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAIStateBase : public UStatusEffectBase
{
	GENERATED_BODY()

public:
	virtual void Activate() override;
	virtual void Activate(float Duration) override;
	virtual void Deactivate() override;
	
};
