// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumAndStruct/FCrackData.h"
#include "EnumAndStruct/FWorldActorData.h"
#include "EnumAndStruct/FWorldInstanceSubsystemData.h"
#include "WorldInstanceSubsystem.generated.h"

class ADropEssence;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecentCrackChanged, int32, NewValue);

UCLASS()
class DOMINIONPROTOCOL_API UWorldInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Share

//Functions
public:
	//Save & Load
	void LoadSaveData(const FWorldInstanceSubsystemData& SaveData);
	FWorldInstanceSubsystemData GetSaveData();
	
	void InitializeCrackAndNewGameDataMap(FCrackData Level1, FCrackData Level2);
	
	//Setter
	FORCEINLINE void SetIsNewGame(FString LevelName, bool bNewBool) { IsNewGameDataMap[LevelName] = bNewBool; }
	
	void SetCrackDataMap(TMap<FString, FCrackDataArrayStruct> InCrackDataMap) { CrackDataMap = InCrackDataMap; }

	FORCEINLINE void SetCurrentLevelName(const FString& NewCurrentLevelName) { CurrentLevelName = NewCurrentLevelName; }
	
	FORCEINLINE void SetCurrentLevelDisplayName(const FText& NewCurrentLevelDisplayName) { CurrentLevelDisplayName = NewCurrentLevelDisplayName; }
	
	FORCEINLINE void SetRecentCrackName(const FText& NewRecentCrackName) { RecentCrackName = NewRecentCrackName; }
	
	void SetRecentCrackIndex(int32 NewRecentCrackIndex);

	FORCEINLINE void SetMaxCrackIndex(int32 CrackIndex) { MaxCrackIndex = CrackIndex; }
	
	FORCEINLINE void SetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) { CrackDataMap[LevelName].CrackDataArray[InCrackIndex].bIsActivate = true; }

	FORCEINLINE void SetMoveTargetLocation(const FVector& NewMoveTargetLocation) { MoveTargetLocation = NewMoveTargetLocation; }
	
	FORCEINLINE void SetMoveTargetRotator(const FRotator& NewMoveTargetRotation) { MoveTargetRotation = NewMoveTargetRotation; }

	FORCEINLINE void SwitchIsLevelChanged() { bIsLevelChanged = !bIsLevelChanged; }

	FORCEINLINE void SetDropEssenceCache(ADropEssence* NewDropEssence) { DropEssenceCache = NewDropEssence; }
	
	FORCEINLINE void SetDropEssenceAmount(int32 NewDropEssenceAmount) { DropEssenceAmount = NewDropEssenceAmount; }

	FORCEINLINE void SetIsDropEssenceExist(bool bExist) { bIsDropEssenceExist = bExist; }

	FORCEINLINE void SetDropEssenceLocation(const FVector& NewDropEssenceLocation) { DropEssenceLocation = NewDropEssenceLocation; }

	FORCEINLINE void SetDropEssenceLocationLevel(const FString& NewDropEssenceLocationLevel) { DropEssenceLocationLevel = NewDropEssenceLocationLevel; }
	
	FORCEINLINE void SetWorldActorDataMap(TMap<FGuid, FWorldActorData>& NewWorldActorDataMap) { WorldActorDataMap.Append(NewWorldActorDataMap); }

	FORCEINLINE void SetWorldActorData(FGuid ID, FWorldActorData NewWorldActorData)
	{
		WorldActorDataMap.Add(ID, NewWorldActorData);
	}

	//Getter
	FORCEINLINE bool GetIsNewGame() const
	{
		if (IsNewGameDataMap.Contains(CurrentLevelName))
		{
			return IsNewGameDataMap[CurrentLevelName];
		}
		return true;
	}
	
	UFUNCTION(BlueprintPure)
	TMap<FString, FCrackDataArrayStruct> GetCrackDataMapForBlueprint() { return CrackDataMap; }
	
	TMap<FString, FCrackDataArrayStruct>* GetCrackDataMap() { return &CrackDataMap; }

	UFUNCTION(BlueprintPure)
	const FString& GetCurrentLevelName() const { return CurrentLevelName; }
	
	FORCEINLINE const FText& GetCurrentLevelDisplayName() const { return CurrentLevelDisplayName; }

	UFUNCTION(BlueprintPure)
	const FText& GetRecentCrackName() const { return RecentCrackName; }
	
	FORCEINLINE int32 GetRecentCrackIndex() const { return RecentCrackIndex; }

	FORCEINLINE int32 GetMaxCrackIndex() const { return MaxCrackIndex; }

	bool GetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) const;

	const FCrackData* GetCrackData(const FString& LevelName, int32 InCrackIndex) const;
	
	FORCEINLINE const FVector& GetMoveTargetLocation() const { return MoveTargetLocation; }
	
	FORCEINLINE const FRotator& GetMoveTargetRotation() const { return MoveTargetRotation; }

	FORCEINLINE bool GetIsLevelChanged() const { return bIsLevelChanged; }

	FORCEINLINE TObjectPtr<ADropEssence> GetDropEssenceCache() const { return DropEssenceCache; }
	
	FORCEINLINE int32 GetDropEssenceAmount() const { return DropEssenceAmount; }

	FORCEINLINE bool GetIsDropEssenceExist() const { return bIsDropEssenceExist; }

	FORCEINLINE const FVector& GetDropEssenceLocation() const { return DropEssenceLocation; }

	FORCEINLINE const FString& GetDropEssenceLocationLevel() const { return DropEssenceLocationLevel; }
	
	FORCEINLINE bool GetIsWorldActorDataExist(FGuid ActorID) const
	{
		if (WorldActorDataMap.Contains(ActorID))
		{
			return true;
		}
		return false;
	}

	FORCEINLINE FWorldActorData GetWorldActorData(FGuid ActorID) const
	{
		if (WorldActorDataMap.Contains(ActorID))
		{
			return WorldActorDataMap[ActorID];
		}
		return FWorldActorData();
	}
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

// Variables
public:
	UPROPERTY(BlueprintAssignable)
	FOnRecentCrackChanged OnRecentCrackChanged;

//SaveData
private:
	UPROPERTY()
	TMap<FString, bool> IsNewGameDataMap;
	
	UPROPERTY()
	FString CurrentLevelName;

	UPROPERTY()
	FText CurrentLevelDisplayName;

	UPROPERTY()
	FText RecentCrackName;

	UPROPERTY()
	int32 RecentCrackIndex = 0;

	UPROPERTY()
	int32 MaxCrackIndex = 1;

	UPROPERTY()
	TMap<FString, FCrackDataArrayStruct> CrackDataMap;

	UPROPERTY()
	TMap<FGuid, FWorldActorData> WorldActorDataMap;
	
	UPROPERTY()
	bool bIsDropEssenceExist;
	
	UPROPERTY()
	int32 DropEssenceAmount;

	UPROPERTY()
	FVector DropEssenceLocation;

	UPROPERTY()
	FString DropEssenceLocationLevel;

//Not Save Variables
private:
	UPROPERTY()
	TObjectPtr<ADropEssence> DropEssenceCache;
	
	FVector MoveTargetLocation = FVector::ZeroVector;

	FRotator MoveTargetRotation = FRotator::ZeroRotator;

	bool bIsLevelChanged = false;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
