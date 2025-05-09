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
	FORCEINLINE void SetIsBossDead(int32 BossIndex);
	FORCEINLINE void SetCurrentLevelName(FString NewCurrentLevelName) { CurrentLevelName = NewCurrentLevelName; }
	FORCEINLINE void SetCurrentLevelDisplayName(FText NewCurrentLevelDisplayName) { CurrentLevelDisplayName = NewCurrentLevelDisplayName; }
	FORCEINLINE void SetRecentCrackName(FText NewRecentCrackName) { RecentCrackName = NewRecentCrackName; }
	FORCEINLINE void SetRecentCrackIndex(int32 NewRecentCrackIndex) { RecentCrackIndex = NewRecentCrackIndex; }
	//Getter
	FORCEINLINE bool GetIsBossDead(int32 BossIndex) const;
	FORCEINLINE FString GetCurrentLevelName() const { return CurrentLevelName; }
	FORCEINLINE FText GetCurrentLevelDisplayName() const { return CurrentLevelDisplayName; }
	FORCEINLINE FText GetRecentCrackName() const { return RecentCrackName; }
	FORCEINLINE int32 GetRecentCrackIndex() const { return RecentCrackIndex; }
	
	
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

	static const int32 NumBosses;
};
