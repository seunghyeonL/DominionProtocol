// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusEffects/StatusEffectBase.h"
#include "AIStateBase.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAIStateBase : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsActive() const { return bIsActive; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FGameplayTag StateTag;

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetStateTag() const { return StateTag; }

	virtual void Activate();
	virtual void Deactivate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsActive;
};