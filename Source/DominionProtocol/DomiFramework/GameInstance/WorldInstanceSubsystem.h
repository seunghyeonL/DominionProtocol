// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumAndStruct/FCrackData.h"

#include "WorldInstanceSubsystem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UWorldInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Share
	
public:
	
	//Setter
	void SetCrackDataMap(TMap<FString, FCrackDataArrayStruct> InCrackDataMap) { CrackDataMap = InCrackDataMap; }

	FORCEINLINE void SetCurrentLevelName(const FString& NewCurrentLevelName) { CurrentLevelName = NewCurrentLevelName; }
	
	FORCEINLINE void SetCurrentLevelDisplayName(const FText& NewCurrentLevelDisplayName) { CurrentLevelDisplayName = NewCurrentLevelDisplayName; }
	
	FORCEINLINE void SetRecentCrackName(const FText& NewRecentCrackName) { RecentCrackName = NewRecentCrackName; }
	
	FORCEINLINE void SetRecentCrackIndex(int32 NewRecentCrackIndex) { RecentCrackIndex = NewRecentCrackIndex; }
	
	FORCEINLINE void SetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) { CrackDataMap[LevelName].CrackDataArray[InCrackIndex].bIsActivate = true; }

	FORCEINLINE void SetMoveTargetLocation(const FVector& NewMoveTargetLocation) { MoveTargetLocation = NewMoveTargetLocation; }
	
	FORCEINLINE void SetMoveTargetRotator(const FRotator& NewMoveTargetRotation) { MoveTargetRotation = NewMoveTargetRotation; }

	FORCEINLINE void SwitchIsLevelChanged() { bIsLevelChanged = !bIsLevelChanged; }
	
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

	bool GetIsActivateCrackIndex(const FString& LevelName, int32 InCrackIndex) const;

	const FCrackData* GetCrackData(const FString& LevelName, int32 InCrackIndex) const;
	
	FORCEINLINE const FVector& GetMoveTargetLocation() const { return MoveTargetLocation; }
	
	FORCEINLINE const FRotator& GetMoveTargetRotation() const { return MoveTargetRotation; }

	FORCEINLINE bool GetIsLevelChanged() const { return bIsLevelChanged; }
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

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
	TMap<FString, FCrackDataArrayStruct> CrackDataMap;

	FVector MoveTargetLocation = FVector::ZeroVector;

	FRotator MoveTargetRotation = FRotator::ZeroRotator;

	bool bIsLevelChanged = false;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
