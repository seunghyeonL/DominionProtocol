// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EnumAndStruct/FInstanceData.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "GameplayTagContainer.h"
#include "DomiGameInstance.generated.h"

class ACrack;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoryStateChanged, EGameStoryState, NewState);

UCLASS()
class DOMINIONPROTOCOL_API UDomiGameInstance : public UGameInstance
{
	GENERATED_BODY()

#pragma region Share
	
public:
	virtual void Init() override;
	
	//Save & Load
	void LoadSaveData(const FInstanceData& SaveData);
	FInstanceData GetSaveData() const;
	
	//Setter
	void SetIsBossDead(FGameplayTag BossTag);
	
	void SetCurrentGameStoryState(EGameStoryState NewGameStoryState);

	FORCEINLINE void SetPlayerCurrentEssence(int32 NewPlayerCurrentEssence) { PlayerCurrentEssence = NewPlayerCurrentEssence; }

	FORCEINLINE void AddPlayerCurrentEssence(int32 AddEssenceValue) { PlayerCurrentEssence += AddEssenceValue; }
	
	FORCEINLINE void SubtractPlayerCurrentEssence(int32 SubtractEssenceValue) { PlayerCurrentEssence -= SubtractEssenceValue; }
	
	//Getter
	UFUNCTION(BlueprintCallable)
	int32 GetPlayerCurrentEssence() const { return PlayerCurrentEssence; }
	
	bool GetIsBossDead(FGameplayTag BossTag) const;
	
	// UI에서 각 균열 활성화/비활성화 정보 담는 배열(각 레벨별로) 반환
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameStoryState GetCurrentGameStoryState() const { return CurrentGameStoryState; }

protected:
	
	
private:
	UPROPERTY()
	TSet<FGameplayTag> DeadBossTags;

	UPROPERTY()
	EGameStoryState CurrentGameStoryState;

#pragma endregion


#pragma region KyuHyeok

private:
	int32 PlayerCurrentEssence;
	
#pragma endregion


#pragma region SeoYoung
public:
	UPROPERTY(BlueprintReadOnly, BlueprintAssignable)
	FOnStoryStateChanged OnStoryStateChanged;

	UFUNCTION(BlueprintCallable)
	void AdvanceStoryState();

#pragma endregion
};