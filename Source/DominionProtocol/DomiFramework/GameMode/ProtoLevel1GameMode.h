// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "Engine/TriggerBox.h"
#include "WorldObjects/ZoneBarrier.h"
#include "ProtoLevel1GameMode.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API AProtoLevel1GameMode : public ABaseGameMode
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

	void StartBattle() override;
	void EndBattle() override;
		
public:
	UPROPERTY(EditAnywhere)
	AZoneBarrier* ZoneBarrier;
};
