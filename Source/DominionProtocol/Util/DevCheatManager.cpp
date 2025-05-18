// Fill out your copyright notice in the Description page of Project Settings.


#include "DevCheatManager.h"
#include "Util/CheatBPLib.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameMode/BaseGameMode.h"

void UDevCheatManager::Save()
{
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		UCheatBPLib::Save(World);
	}
}

void UDevCheatManager::InfiniteStamina()
{
	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(GetOuterAPlayerController()->GetPawn());
	TObjectPtr<UStatusComponent> StatusComponent = PlayerCharacter->GetStatusComponent();
	if (IsValid(StatusComponent))
	{
		UCheatBPLib::InfiniteStamina(StatusComponent);
	}
}

void UDevCheatManager::MoveToCrack(FString TargetLevelName, int32 TargetCrackIndex)
{
	ABaseGameMode* BaseGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	UCheatBPLib::MoveToCrack(BaseGameMode, TargetLevelName, TargetCrackIndex);
}
