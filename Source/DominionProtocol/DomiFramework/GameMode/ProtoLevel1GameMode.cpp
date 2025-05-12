// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Util/DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Characters/DomiCharacter.h"
#include "Engine/TargetPoint.h"

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
	Debug::Print(TEXT("StartBattle"));

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
				if (BossClass)
				{
					TArray<AActor*> BossSpawnTargetPoints;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATargetPoint::StaticClass(), BossSpawnTargetPoints);

					FTransform SpawnTransform;

					for (AActor* Point : BossSpawnTargetPoints)
					{
						if (IsValid(Point))
						{
							if (Point->GetActorLabel().Contains("BossSpawnPoint"))
							{
								Debug::Print(Point->GetActorTransform().ToString());
								SpawnTransform = Point->GetActorTransform();
								BossSpawnPoint = Cast<ATargetPoint>(Point);
							}
						}
					}
					
					ABaseEnemy* SpawnedBoss = GetWorld()->SpawnActor<ABaseEnemy>(BossClass,SpawnTransform);

					if (IsValid(SpawnedBoss))
					{
						Debug::Print(TEXT("Boss Spawn"));
					}
					else
					{
						Debug::Print(TEXT("Failed Boss Spawn"));
					}
				}
				else
				{
					Debug::Print(TEXT("Invalid BossClass"));
				}
			},
			0.5f, false);
	}
	else
	{
		Debug::Print(TEXT("Boss Already Dead"));
	}
}

void AProtoLevel1GameMode::EndBattle()
{
	Super::EndBattle();
	Debug::Print(TEXT("EndBattle"));
	
	GameInstance->SetIsBossDead(0);

	if (ZoneBarrier)
	{
		ZoneBarrier->DeactivateBarrier();
	}
}
