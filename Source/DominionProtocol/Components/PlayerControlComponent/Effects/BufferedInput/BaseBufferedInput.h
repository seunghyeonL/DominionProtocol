// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "BaseBufferedInput.generated.h"

class UPlayerControlComponent;
/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UBaseBufferedInput : public UObject
{
	GENERATED_BODY()
	
protected:
	bool bIsExpired;
	FTimerHandle ValidTimerHandle;
	float ExpireDuration;
	
	UPROPERTY()
	TObjectPtr<UPlayerControlComponent> ControlComponent;

public:
	UBaseBufferedInput();
	void SetTimer();
	virtual void Operate();
	FORCEINLINE bool IsExpired() const { return bIsExpired; }
};
