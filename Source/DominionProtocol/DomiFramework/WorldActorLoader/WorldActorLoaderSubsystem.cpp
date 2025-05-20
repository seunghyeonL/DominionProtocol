// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActorLoaderSubsystem.h"

#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Util/DebugHelper.h"
#include "WorldObjects/RegionPassWall.h"
#include "EnumAndStruct/ECategory.h"
#include "EnumAndStruct/FWorldActorData.h"

void UWorldActorLoaderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	WorldInstanceSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
	check(WorldInstanceSubsystem);
}

void UWorldActorLoaderSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UWorldActorLoaderSubsystem::UpdateActivateRegions(int32 RegionIndex)
{
	for (int32 i = 0; i <= MaxRegion; i++)
	{
		ActivateRegionArray[i] = false;
	}

	ActivateRegionArray[RegionIndex] = true;
	if (RegionIndex > 0)
	{
		ActivateRegionArray[RegionIndex - 1] = true;
	}

	if (RegionIndex < MaxRegion)
	{
		ActivateRegionArray[RegionIndex + 1] = true;
	}
}


void UWorldActorLoaderSubsystem::BindDelegateToWorldActorLoader()
{
	WorldInstanceSubsystem->OnRecentCrackChanged.AddDynamic(this, &UWorldActorLoaderSubsystem::HandleRegionChange);

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARegionPassWall::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ARegionPassWall* RegionPassWall = Cast<ARegionPassWall>(Actor);
		if (!IsValid(RegionPassWall))
		{
			Debug::Print(TEXT(
				"UWorldActorLoaderSubsystem::BindDelegateToWorldActorLoader : Some RegionPassWall is invalid"));
			continue;
		}
		RegionPassWall->OnRegionChanged.AddDynamic(this, &UWorldActorLoaderSubsystem::HandleRegionChange);
	}
}

void UWorldActorLoaderSubsystem::SpawnActorToTargetTransform(int32 LocatedRegionIndex, FGuid InUniqueID,
                                                             TSubclassOf<AActor> ActorClass,
                                                             EWorldActorCategory ActorCategory,
                                                             const FVector& Location, const FRotator& Rotation)
{
	if (ActorClass == nullptr || ActorCategory == EWorldActorCategory::None)
	{
		Debug::PrintError(TEXT("UWorldActorLoaderSubsystem::SpawnActorToTargetTransform : ActorLcass or ActorCategory is invalid"));
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* NewActor = GetWorld()->SpawnActor(ActorClass, &Location, &Rotation, SpawnParams);
	checkf(NewActor, TEXT("NewActor is nullptr"));
	
	FActivateActorData NewActivateActorData;
	NewActivateActorData.Actor = NewActor;
	NewActivateActorData.LocatedRegionIndex = LocatedRegionIndex;
	ActivateActorsCacheMap.Add(InUniqueID, NewActivateActorData);
	
	FWorldActorData* FoundActorData = RegionDataMap[LocatedRegionIndex].WorldActorArray.FindByPredicate(
		[InUniqueID](const FWorldActorData& ActorData)
		{
			return ActorData.InstanceGuid == InUniqueID;
		});
	if (FoundActorData)
	{
		FoundActorData->bIsActivate = true;
	}
	else
	{
		Debug::PrintError(FString::Printf(TEXT("UWorldActorLoaderSubsystem::SpawnActorToTargetTransform : Actor data not found for GUID %s"), *InUniqueID.ToString()));
	}

	// 액터 카테고리에 따른 데이터 넣어주기(합의 후 작성 필요함)
	switch (ActorCategory)
	{
	case EWorldActorCategory::None:
		break;
	case EWorldActorCategory::Item:
		break;
	case EWorldActorCategory::Interactable:
		break;
	case EWorldActorCategory::Enemy:
		break;
	default:
		break;
	}
}

void UWorldActorLoaderSubsystem::HandleRegionChange(int32 RegionIndex)
{
	Debug::Print(FString::Printf(TEXT("Current Region %d"), RegionIndex));
	UpdateActivateRegions(RegionIndex);
	
	ActiveRegion(RegionIndex);
	DeactiveRegion(RegionIndex);
}

void UWorldActorLoaderSubsystem::ActiveRegion(int32 RegionIndex)
{
	for (int32 i = 0; i <= MaxRegion; i++)
	{
		if (!RegionDataMap.Contains(i))
		{
			Debug::PrintError(FString::Printf(TEXT("UWorldActorLoaderSubsystem::ActiveRegion : Region %d is not in ActivateRegions"), i));
			continue;
		}
		if (!ActivateRegionArray[i])
		{
			continue;
		}
		
		TArray<FWorldActorData>& RegionActorDataArray = RegionDataMap[i].WorldActorArray;

		for (FWorldActorData& RegionActorData : RegionActorDataArray)
		{
			if (RegionActorData.bIsActivate)
			{
				continue;
			}
			
			SpawnActorToTargetTransform(
				i,
				RegionActorData.InstanceGuid,
				RegionActorData.LinkedActorClass,
				RegionDataMap[i].ActorCategory,
				RegionActorData.Location,
				RegionActorData.Rotation);
		}
	}
}

void UWorldActorLoaderSubsystem::DeactiveRegion(int32 RegionIndex)
{
	//WorldInstanceSubsystem->UpdateWorldActorData(RegionIndex);

	for (int32 i = 0; i <= MaxRegion; i++)
	{
		if (!RegionDataMap.Contains(i))
		{
			Debug::PrintError(FString::Printf(TEXT("UWorldActorLoaderSubsystem::DeactiveRegion : Region %d is not in ActivateRegions"), i));
			continue;
		}
		if (ActivateRegionArray[i])
		{
			continue;
		}

		for (FWorldActorData& ActorData : RegionDataMap[i].WorldActorArray)
		{
			if (ActorData.bIsActivate)
			{
				ActorData.bIsActivate = false;

				FActivateActorData* ActivateActorData = ActivateActorsCacheMap.Find(ActorData.InstanceGuid);
				if (ActivateActorData && IsValid(ActivateActorData->Actor))
				{
					ActivateActorData->Actor->Destroy();
					ActivateActorData->Actor = nullptr;
					ActivateActorsCacheMap.Remove(ActorData.InstanceGuid);
				}
			}
		}
	}
}

bool UWorldActorLoaderSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	check(World);

	// WorldType이 Editor Preview같은 다른 타입인 경우에는 생성하지 않음
	if (World->WorldType != EWorldType::Game && World->WorldType != EWorldType::PIE)
	{
		return false;
	}

	// 이름 기준으로 필요없는 레벨에선 서브시스템 생성하지 않음
	const FString WorldName = World->GetName();
	if (WorldName.Contains("Title") || WorldName.Contains("MainMenu"))
	{
		return false;
	}

	return true;
}
