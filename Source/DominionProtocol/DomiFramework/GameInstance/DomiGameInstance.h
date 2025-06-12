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

	FORCEINLINE bool HasEnoughEssence(const float InEssence) const { return InEssence <= PlayerCurrentEssence; }
	
	//Setter
	FORCEINLINE void SetWorldCache(UWorld* NewWorld) { World = NewWorld; }

	FORCEINLINE void SetSaveSlotName(const FString& SlotName) { SaveSlotName = SlotName; }

	FORCEINLINE void SetSaveSlotIndex(int32 SlotIndex) { SaveSlotIndex = SlotIndex; }
	
	void SetIsBossDead(FGameplayTag BossTag);
	
	void SetCurrentGameStoryState(EGameStoryState NewGameStoryState);

	FORCEINLINE void SetPlayerCurrentEssence(int32 NewPlayerCurrentEssence) { PlayerCurrentEssence = NewPlayerCurrentEssence; }

	FORCEINLINE void AddPlayerCurrentEssence(int32 AddEssenceValue) { PlayerCurrentEssence += AddEssenceValue; }
	
	FORCEINLINE void SubtractPlayerCurrentEssence(int32 SubtractEssenceValue)
	{
		if (PlayerCurrentEssence < SubtractEssenceValue)
		{
			return;
		}
		PlayerCurrentEssence = FMath::Max(0, PlayerCurrentEssence - SubtractEssenceValue);
	}

	FORCEINLINE void SetStatDataMap(TMap<FGameplayTag, float> NewStatDataMap) { StatDataMap.Append(NewStatDataMap); }

	FORCEINLINE void AddPlayTime(int32 NewPlayTime) { PlayTime += NewPlayTime; }
	
	//Getter
	FORCEINLINE const FString& GetSaveSlotName() const { return SaveSlotName; }

	FORCEINLINE int32 GetSaveSlotIndex() const { return SaveSlotIndex; }
	
	UFUNCTION(BlueprintCallable)
	int32 GetPlayerCurrentEssence() const { return PlayerCurrentEssence; }

	FORCEINLINE const TMap<FGameplayTag, float>& GetStatDataMap() const { return StatDataMap; }
	
	bool GetIsBossDead(FGameplayTag BossTag) const;

	FORCEINLINE int32 GetPlayTime() const { return PlayTime; }
	
	// UI에서 각 균열 활성화/비활성화 정보 담는 배열(각 레벨별로) 반환
	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameStoryState GetCurrentGameStoryState() const { return CurrentGameStoryState; }

protected:
	
	
private:
	UPROPERTY()
	FString SaveSlotName;

	UPROPERTY()
	int32 SaveSlotIndex;
	
	UPROPERTY()
	TSet<FGameplayTag> DeadBossTags;

	UPROPERTY()
	TMap<FGameplayTag, float> StatDataMap;

	UPROPERTY()
	EGameStoryState CurrentGameStoryState;

	UPROPERTY()
	int32 PlayTime;

	//Not Save Data
	UPROPERTY()
	TObjectPtr<UWorld> World;

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

	UFUNCTION(BlueprintCallable)
	bool ReturnStoryState();

#pragma endregion
};