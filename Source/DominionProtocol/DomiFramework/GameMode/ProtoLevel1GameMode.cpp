// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiFramework/GameMode/ProtoLevel1GameMode.h"
#include "Util/DebugHelper.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Characters/DomiCharacter.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "Engine/TargetPoint.h"
#include "AI/ProtoBoss/ProtoBossEnemy.h"
#include "EngineUtils.h"
#include "WorldObjects/Crack.h"


void AProtoLevel1GameMode::StartPlay()
{
	Super::StartPlay();

	// if (!GameInstance) return;
	//
	// int32 TargetCrackIndex = WorldInstanceSubsystem->GetRecentCrackIndex();
	//
	// Debug::Print(TEXT("=====") + TargetCrackIndex);
	// if (TargetCrackIndex == -1)
	// {
	// 	Debug::Print(TEXT("StartPlay: CrackIndex 없음 → 기본 위치 스폰")+ TargetCrackIndex);
	// 	return;
	// }
	//
	// for (TActorIterator<ACrack> It(GetWorld()); It; ++It)
	// {
	// 	ACrack* Crack = *It;
	// 	if (Crack->GetCrackIndex() == TargetCrackIndex)
	// 	{
	// 		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// 		if (IsValid(PlayerPawn))
	// 		{
	// 			PlayerPawn->SetActorLocation(Crack->GetRespawnTargetPointLocation());
	// 			PlayerPawn->SetActorRotation(Crack->GetRespawnTargetPointRotation());
	//
	// 			Debug::Print(FString::Printf(TEXT("CrackIndex %d 위치로 이동 완료"), TargetCrackIndex));
	// 		}
	// 		//return;
	// 	}
	// }

	//Debug::PrintError(TEXT("StartPlay: 해당 CrackIndex에 맞는 균열을 찾지 못했습니다."));

	ZoneBarrier = Cast<AZoneBarrier>(UGameplayStatics::GetActorOfClass(GetWorld(), AZoneBarrier::StaticClass()));

	if (IsValid(ZoneBarrier))
	{
		// ZoneBarrier 델리게이트
		ZoneBarrier->OnPlayerEnterZoneDelegate.AddDynamic(this, &AProtoLevel1GameMode::StartBattle);
		Debug::Print(TEXT("GM1: Delegate ZoneBarrier"));


		if (GameInstance->GetIsBossDead(0))
		{
			ZoneBarrier->DeactivateBarrier();
		}
		else
		{
			Debug::Print(TEXT("GM1: Boss Already Dead"));
		}
	}
	else
	{
		Debug::Print(TEXT("GM1: Zone Barrier Not Valid"));
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
					
					AProtoBossEnemy* SpawnedBoss = GetWorld()->SpawnActor<AProtoBossEnemy>(BossClass,SpawnTransform);

					if (IsValid(SpawnedBoss))
					{
						Debug::Print(TEXT("Boss Spawn"));
						SpawnedBoss->OnBossDeathDelegate.AddDynamic(this, &AProtoLevel1GameMode::EndBattle);
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

void AProtoLevel1GameMode::OnPlayerDeath()
{
	Super::OnPlayerDeath();
}
