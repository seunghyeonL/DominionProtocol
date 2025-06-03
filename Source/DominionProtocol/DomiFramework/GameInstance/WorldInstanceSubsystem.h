// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumAndStruct/FCrackData.h"

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
	void InitializeCrackDataMap(FCrackData Level1, FCrackData Level2);
	
	//Setter
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
	//Getter
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
	TObjectPtr<ADropEssence> DropEssenceCache;
	
	UPROPERTY()
	bool bIsDropEssenceExist;
	
	UPROPERTY()
	int32 DropEssenceAmount;

	UPROPERTY()
	FVector DropEssenceLocation;

//Not Save Variables
private:
	FVector MoveTargetLocation = FVector::ZeroVector;

	FRotator MoveTargetRotation = FRotator::ZeroRotator;

	bool bIsLevelChanged = false;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
