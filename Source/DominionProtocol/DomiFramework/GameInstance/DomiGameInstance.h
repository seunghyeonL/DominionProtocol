// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EnumAndStruct/FInstanceData.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "EnumAndStruct/FCrackData.h"
#include "DomiGameInstance.generated.h"

class ACrack;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoryStateChanged, EGameStoryState, NewState);

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
	void SetIsBossDead(int32 BossIndex);
	
	void SetCurrentGameStoryState(EGameStoryState NewGameStoryState);

	
	//Getter
	bool GetIsBossDead(int32 BossIndex) const;
	
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
	EGameStoryState CurrentGameStoryState;
	
	static const int32 NumBosses;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

	UPROPERTY(BlueprintAssignable)
	FOnStoryStateChanged OnStoryStateChanged;

#pragma endregion
};