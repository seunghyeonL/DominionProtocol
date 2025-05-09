// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "StatusEffectBase.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UStatusEffectBase : public UObject
{
	GENERATED_BODY()

protected:
	FTimerHandle DurationTimer;
	FTimerHandle DoTTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	FGameplayTag StatusEffectTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsActive;

public:
	virtual void Activate();
	virtual void Activate(float Duration);
	virtual void Deactivate();
	virtual void Tick(float DeltaTime);
};
