// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "BaseGameMode.generated.h"

class ADomiCharacter;
class ACrack;

UCLASS()
class DOMINIONPROTOCOL_API ABaseGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABaseGameMode();
	
	virtual void StartPlay() override;
	virtual void StartBattle();
	virtual void EndBattle();

	void OnPlayerDeath();
	void RespawnPlayerCharacter();

	//Getter
	
	//Setter
	FORCEINLINE void SetRecentCrackCache(ACrack* NewCrack) { RecentCrackCache = NewCrack ; } 

protected:
	UPROPERTY()
	UDomiGameInstance* GameInstance;

	UPROPERTY()
	ADomiCharacter* PlayerCharacter;
	
	UPROPERTY()
	ACrack* RecentCrackCache;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RespawnDelay;
	
	FString CurrentLevel;
};
