// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/Effects/PlayerControlEffectBase.h"
#include "PlayerPreStunEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerPreStunEffect : public UPlayerControlEffectBase
{
	GENERATED_BODY()
	
protected:
	float DurationPerAttack;
	

public:
	UPlayerPreStunEffect();

	virtual bool Activate() override;
	virtual bool Activate(float Duration) override;
	virtual void Deactivate() override;

	virtual void Tick(float DeltaTime) override;
};
