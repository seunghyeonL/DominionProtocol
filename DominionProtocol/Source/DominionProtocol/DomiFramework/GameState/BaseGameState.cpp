// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"

#include "Util/DebugHelper.h"

void ABaseGameState::BeginPlay()
{
	Super::BeginPlay();

	SoundSubsystem = GetGameInstance()->GetSubsystem<USoundInstanceSubsystem>();
	SoundSubsystem->LoadVolumeSettings();
}
