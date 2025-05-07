// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BaseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABaseGameMode();
	virtual void StartPlay() override;

private:
	UGameInstance* GameInstance;
	FName CurrentLevel;
};
