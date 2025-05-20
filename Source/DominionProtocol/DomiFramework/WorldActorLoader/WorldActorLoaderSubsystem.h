// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/ECategory.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnumAndStruct/FWorldActorData.h"
#include "WorldActorLoaderSubsystem.generated.h"

class UWorldInstanceSubsystem;

UCLASS()
class DOMINIONPROTOCOL_API UWorldActorLoaderSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

//Functions
public:
	UFUNCTION()
	void HandleRegionChange(int32 RegionIndex);
	
	//Setter
	FORCEINLINE void SetRegionDataMap(TMap<int32, FRegionActorData> NewRegionDataMap) { RegionDataMap = NewRegionDataMap; }

	FORCEINLINE void SetMaxRegion(int32 InMaxRegion) { MaxRegion = InMaxRegion; }

	FORCEINLINE void SetActivateRegionArray(TArray<bool> NewActivateRegionArray) { ActivateRegionArray = NewActivateRegionArray; }
	
	//Getter
	FORCEINLINE TMap<int32, FRegionActorData> GetRegionDataMap() const { return RegionDataMap; }

	FORCEINLINE int32 GetMaxRegion() const { return MaxRegion; }


	void UpdateActivateRegions(int32 RegionIndex);
	
	void ActiveRegion(int32 RegionIndex);
	
	void DeactiveRegion(int32 RegionIndex);
	
	void BindDelegateToWorldActorLoader();
	
	void SpawnActorToTargetTransform(int32 LocatedRegionIndex, FGuid InUniqueID, TSubclassOf<AActor> ActorClass, EWorldActorCategory ActorCategory, const FVector& Location, const FRotator& Rotation);
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	

//Variables
private:
	UPROPERTY()
	UWorldInstanceSubsystem* WorldInstanceSubsystem;
	
	UPROPERTY()
	TMap<int32, FRegionActorData> RegionDataMap;
	
	UPROPERTY()
	TMap<FGuid, FActivateActorData> ActivateActorsCacheMap;

	int32 MaxRegion = 1;

	TArray<bool> ActivateRegionArray = {};
};