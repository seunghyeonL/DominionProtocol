// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Util/DebugHelper.h"

void AProtoLevel1GameMode::StartPlay()
{
	Super::StartPlay();

	if (GameInstance->GetIsBossDead(0) && ZoneBarrier)
	{
		//ZoneBarrier->DeactivateBarrier();
	}
}

void AProtoLevel1GameMode::StartBattle()
{
	Super::StartBattle();

	Debug::Print(TEXT("Start Battle"));

	if (GameInstance->GetIsBossDead(0) && ZoneBarrier)
	{
		ZoneBarrier->ActivateBarrier();
	}
}

void AProtoLevel1GameMode::EndBattle()
{
	Super::EndBattle();
	
	Debug::Print(TEXT("End Battle"));

	GameInstance->SetIsBossDead(0);

	if (ZoneBarrier)
	{
		ZoneBarrier->DeactivateBarrier();
	}
}
