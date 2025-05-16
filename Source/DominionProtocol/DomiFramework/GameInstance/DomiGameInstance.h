// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EnumAndStruct/FInstanceData.h"
#include "DomiGameInstance.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UDomiGameInstance();

	//Save & Load
	void LoadSaveData(const FInstanceData& SaveData);
	FInstanceData GetSaveData() const;
	
	//Setter
	void SetIsBossDead(int32 BossIndex);
	FORCEINLINE void SetCurrentLevelName(FString NewCurrentLevelName) { CurrentLevelName = NewCurrentLevelName; }
	FORCEINLINE void SetCurrentLevelDisplayName(FText NewCurrentLevelDisplayName) { CurrentLevelDisplayName = NewCurrentLevelDisplayName; }
	FORCEINLINE void SetRecentCrackName(FText NewRecentCrackName) { RecentCrackName = NewRecentCrackName; }
	FORCEINLINE void SetRecentCrackIndex(int32 NewRecentCrackIndex) { RecentCrackIndex = NewRecentCrackIndex; }
	void SetIsActivateCrack(int32 InCrackIndex);
	
	//Getter
	bool GetIsBossDead(int32 BossIndex) const;
	FORCEINLINE FString GetCurrentLevelName() const { return CurrentLevelName; }
	FORCEINLINE FText GetCurrentLevelDisplayName() const { return CurrentLevelDisplayName; }
	FORCEINLINE FText GetRecentCrackName() const { return RecentCrackName; }
	FORCEINLINE int32 GetRecentCrackIndex() const { return RecentCrackIndex; }
	//균열에서 초기에 자기 자신이 활성화/비활성화 상태인지 세팅을 위한 Getter(균열의 BeginPlay에서 호출해서 세팅)
	FORCEINLINE bool GetIsActivatePastCrack(int32 InCrackIndex) const { return PastCrackActivateArray.IsValidIndex(InCrackIndex) ? PastCrackActivateArray[InCrackIndex] : false; }
	FORCEINLINE bool GetIsActivatePresentCrack(int32 InCrackIndex) const { return PresentCrackActivateArray.IsValidIndex(InCrackIndex) ? PresentCrackActivateArray[InCrackIndex] : false; }
	// UI에서 각 균열 활성화/비활성화 정보 담는 배열(각 레벨별로) 반환
	FORCEINLINE TArray<bool> GetPastCrackActivateArray() const { return PastCrackActivateArray; }
	FORCEINLINE TArray<bool> GetPresentCrackActivateArray() const { return PresentCrackActivateArray; }

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
	int32 RecentCrackIndex;

	// Proto_Level1(임시)
	UPROPERTY()
	TArray<bool> PastCrackActivateArray;

	// Proto_Level2(임시)
	UPROPERTY()
	TArray<bool> PresentCrackActivateArray;

	static const int32 NumBosses;
};
