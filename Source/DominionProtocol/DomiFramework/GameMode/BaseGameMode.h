// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
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
	virtual void StartBattle();
	virtual void EndBattle();

public:
	UPROPERTY()
	UDomiGameInstance* GameInstance;
	FName CurrentLevel;
};
