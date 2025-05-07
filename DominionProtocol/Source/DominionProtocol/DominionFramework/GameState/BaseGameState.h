// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

class USoundInstanceSubsystem;

UCLASS()
class DOMINIONPROTOCOL_API ABaseGameState : public AGameState
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	USoundInstanceSubsystem* SoundSubsystem;
};
