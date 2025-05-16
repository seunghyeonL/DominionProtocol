// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoLevel2GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AI/AICharacters/BaseEnemy.h"
#include "WorldObjects/Crack.h"
//#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "Util/DebugHelper.h"

AProtoLevel2GameMode::AProtoLevel2GameMode()
{
}

void AProtoLevel2GameMode::StartPlay()
{
	Super::StartPlay();

	if (!GameInstance) return;

	int32 TargetCrackIndex = GameInstance->GetRecentCrackIndex();
	Debug::Print(TEXT("=====") + TargetCrackIndex);
	if (TargetCrackIndex == -1)
	{
		Debug::Print(TEXT("StartPlay: CrackIndex 없음 → 기본 위치 스폰"));
		return;
	}

	for (TActorIterator<ACrack> It(GetWorld()); It; ++It)
	{
		ACrack* Crack = *It;
		if (Crack->GetCrackIndex() == TargetCrackIndex)
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
			if (IsValid(PlayerPawn))
			{
				PlayerPawn->SetActorLocation(Crack->GetRespawnTargetPointLocation());
				PlayerPawn->SetActorRotation(Crack->GetRespawnTargetPointRotation());
				Debug::Print(FString::Printf(TEXT("CrackIndex %d 위치로 이동 완료"), TargetCrackIndex));
			}
			//return;
		}
	}
}

void AProtoLevel2GameMode::BeginPlay()
{
	Super::BeginPlay();
}