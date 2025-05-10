// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Util/DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Characters/DomiCharacter.h"

void AProtoLevel1GameMode::StartPlay()
{
	Super::StartPlay();

	ZoneBarrier = Cast<AZoneBarrier>(UGameplayStatics::GetActorOfClass(GetWorld(), AZoneBarrier::StaticClass()));

	if (IsValid(ZoneBarrier))
	{
		ZoneBarrier->OnPlayerEnterZoneDelegate.AddDynamic(this, &AProtoLevel1GameMode::StartBattle);

		if (GameInstance->GetIsBossDead(0))
		{
			{
				ZoneBarrier->DeactivateBarrier();
			}
		}
	}
}

void AProtoLevel1GameMode::StartBattle()
{
	Super::StartBattle();

	if (!GameInstance)
	{
		return;
	}

	if (!ZoneBarrier)
	{
		return;
	}

	if (!GameInstance->GetIsBossDead(0))
	{
		FTimerHandle BarrierTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			BarrierTimerHandle,
			[this]()
			{
				if (ZoneBarrier)
				{
					ZoneBarrier->ActivateBarrier();
				}
			},
			1.0f, false);
	}
	else
	{
		Debug::Print(TEXT("Boss Already Dead"));
	}
}

void AProtoLevel1GameMode::EndBattle()
{
	Super::EndBattle();
	
	GameInstance->SetIsBossDead(0);

	if (ZoneBarrier)
	{
		ZoneBarrier->DeactivateBarrier();
	}
}
