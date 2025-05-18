// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EnumAndStruct/FInstanceData.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "EnumAndStruct/FCrackData.h"
#include "DomiGameInstance.generated.h"

class ACrack;

UCLASS()
class DOMINIONPROTOCOL_API UDomiGameInstance : public UGameInstance
{
	GENERATED_BODY()

#pragma region Share
	
public:
	UDomiGameInstance();

	//Save & Load
	void LoadSaveData(const FInstanceData& SaveData);
	FInstanceData GetSaveData() const;
	
	//Setter
	void SetCrackDataMap(TMap<FString, FCrackDataArray> InCrackDataMap) { CrackDataMap = InCrackDataMap; }
	
	void SetIsBossDead(int32 BossIndex);
	
	FORCEINLINE void SetCurrentLevelName(const FString& NewCurrentLevelName) { CurrentLevelName = NewCurrentLevelName; }
	
	FORCEINLINE void SetCurrentLevelDisplayName(const FText& NewCurrentLevelDisplayName) { CurrentLevelDisplayName = NewCurrentLevelDisplayName; }
	
	FORCEINLINE void SetRecentCrackName(const FText& NewRecentCrackName) { RecentCrackName = NewRecentCrackName; }
	
	FORCEINLINE void SetRecentCrackIndex(int32 NewRecentCrackIndex) { RecentCrackIndex = NewRecentCrackIndex; }
	
	FORCEINLINE void SetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) { CrackDataMap[LevelName].CrackDataArray[InCrackIndex].bIsActivate = true; }
	
	FORCEINLINE void SetCurrentGameStoryState(EGameStoryState NewGameStoryState) { CurrentGameStoryState = NewGameStoryState; }
	
	FORCEINLINE void SetMoveTargetLocation(const FVector& NewMoveTargetLocation) { MoveTargetLocation = NewMoveTargetLocation; }
	
	FORCEINLINE void SetMoveTargetRotator(const FRotator& NewMoveTargetRotation) { MoveTargetRotation = NewMoveTargetRotation; }

	FORCEINLINE void SwitchIsLevelChanged() { bIsLevelChanged = !bIsLevelChanged; }

	
	//Getter
	TMap<FString, FCrackDataArray>* GetCrackDataMap() { return &CrackDataMap; }
	
	bool GetIsBossDead(int32 BossIndex) const;
	
	FORCEINLINE const FString& GetCurrentLevelName() const { return CurrentLevelName; }
	
	FORCEINLINE const FText& GetCurrentLevelDisplayName() const { return CurrentLevelDisplayName; }
	
	FORCEINLINE const FText& GetRecentCrackName() const { return RecentCrackName; }
	
	FORCEINLINE int32 GetRecentCrackIndex() const { return RecentCrackIndex; }

	bool GetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) const;

	const FCrackData* GetCrackData(const FString& LevelName, int32 InCrackIndex) const;
	
	FORCEINLINE const FVector& GetMoveTargetLocation() const { return MoveTargetLocation; }
	
	FORCEINLINE const FRotator& GetMoveTargetRotation() const { return MoveTargetRotation; }

	FORCEINLINE bool GetIsLevelChanged() const { return bIsLevelChanged; }
	
	// UI에서 각 균열 활성화/비활성화 정보 담는 배열(각 레벨별로) 반환
	FORCEINLINE EGameStoryState GetCurrentGameStoryState() const { return CurrentGameStoryState; }

protected:
	
private:
	//=========
	//보스 태그 이름 정해지면 bool 말고 태그 이용한 방식으로 변경해야함
	UPROPERTY()
	TArray<bool> IsBossDeadArray;
	//========

	UPROPERTY()
	FString CurrentLevelName;

	UPROPERTY()
	FText CurrentLevelDisplayName;

	UPROPERTY()
	FText RecentCrackName;

	UPROPERTY()
	int32 RecentCrackIndex = 0;

	UPROPERTY()
	TMap<FString, FCrackDataArray> CrackDataMap;

	UPROPERTY()
	EGameStoryState CurrentGameStoryState;

	FVector MoveTargetLocation = FVector::ZeroVector;

	FRotator MoveTargetRotation = FRotator::ZeroRotator;

	bool bIsLevelChanged = false;

	static const int32 NumBosses;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};