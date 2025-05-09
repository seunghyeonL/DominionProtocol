// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Util/DebugHelper.h"
#include "Kismet/GameplayStatics.h"

void AProtoLevel1GameMode::StartPlay()
{
	Super::StartPlay();

	ZoneBarrier = Cast<AZoneBarrier>(UGameplayStatics::GetActorOfClass(GetWorld(), AZoneBarrier::StaticClass()));

	if (GameInstance->GetIsBossDead(0) && IsValid(ZoneBarrier))
	{
		ZoneBarrier->DeactivateBarrier();
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
		Debug::Print(TEXT("Boss Not Dead"));
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
		Debug::Print(TEXT("No"));
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
