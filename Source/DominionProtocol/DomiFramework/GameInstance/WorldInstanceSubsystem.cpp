// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldInstanceSubsystem.h"
#include "EnumAndStruct/FCrackData.h"

#include "Util/DebugHelper.h"

void UWorldInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Level1 크랙 데이터(0번째 균열만)
	{
		FCrackDataArray Level1CrackArray;
		FCrackData Level1CrackData;

		Level1CrackData.OwnerLevelName = FString("Proto_Level1");
		Level1CrackData.CrackIndex = 0;
		Level1CrackData.CrackName = FText::FromString("Level1 Crack0");
		Level1CrackData.bIsActivate = true;
		Level1CrackData.RespawnLocation = FVector(-1860, -1510, 300);
		Level1CrackData.RespawnRotation = FRotator(0, 0, 0);
        
		Level1CrackArray.CrackDataArray.Add(Level1CrackData);
		CrackDataMap.Add(FString("Proto_Level1"), Level1CrackArray);
		
		Level1CrackArray.CrackDataArray[0].OwnerLevelName = FString("TestCrackLevel1");
		CrackDataMap.Add(FString("TestCrackLevel1"), Level1CrackArray);
	}

	// Level2 크랙 데이터(0번째 균열만)
	{
		FCrackDataArray Level2CrackArray;
		FCrackData Level2CrackData;

		Level2CrackData.OwnerLevelName = FString("Proto_Level2");
		Level2CrackData.CrackIndex = 0;
		Level2CrackData.CrackName = FText::FromString("Level2 Crack0");
		Level2CrackData.bIsActivate = true;
		Level2CrackData.RespawnLocation = FVector(-1860, -1510, 300);
		Level2CrackData.RespawnRotation = FRotator(0, 0, 0);
        
		Level2CrackArray.CrackDataArray.Add(Level2CrackData);
		CrackDataMap.Add(FString("Proto_Level2"), Level2CrackArray);

		Level2CrackArray.CrackDataArray[0].OwnerLevelName = FString("TestCrackLevel2");
		CrackDataMap.Add(FString("TestCrackLevel2"), Level2CrackArray);
	}
}

void UWorldInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UWorldInstanceSubsystem::GetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) const
{
	if (const FCrackDataArray* CrackDataArrayRef = CrackDataMap.Find(LevelName))
	{
		Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : CrackDataMap Founded"));
		if (CrackDataArrayRef->CrackDataArray.IsValidIndex(InCrackIndex))
		{
			Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : CrackDataMap is returned!!"));
			return CrackDataArrayRef->CrackDataArray[InCrackIndex].bIsActivate;
		}
	}

	Debug::Print(TEXT("UDomiGameInstance::GetIsActivateCrackIndex : result is nullptr"));
	return false;
}

const FCrackData* UWorldInstanceSubsystem::GetCrackData(const FString& LevelName, int32 InCrackIndex) const
{
	if (const FCrackDataArray* CrackDataArrayRef = CrackDataMap.Find(LevelName))
	{
		if (CrackDataArrayRef->CrackDataArray.IsValidIndex(InCrackIndex))
		{
			return &CrackDataArrayRef->CrackDataArray[InCrackIndex];
		}
	}

	return nullptr;
}
