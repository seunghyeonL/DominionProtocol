// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActorLoaderSubsystem.h"

void UWorldActorLoaderSubsystem::SpawnActorToTargetTransform(TSubclassOf<AActor> ActorClass,
                                                             EWorldActorCategory ActorCategory, FGuid InUniqueID,
                                                             const FVector& Location, const FRotator& Rotation)
{
	if (ActorClass == nullptr || ActorCategory == EWorldActorCategory::None)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (ActorCategory == EWorldActorCategory::Item)
	{
		if (ItemDataMap.Find(InUniqueID) == nullptr)
		{
			FWorldItemData NewItemData;
			NewItemData.UniqueID = InUniqueID;
			NewItemData.ActorClass = ActorClass;
			NewItemData.bIsCollected = false;
			NewItemData.Location = Location;
			NewItemData.Rotation = Rotation;

			ItemDataMap.Add(InUniqueID, NewItemData);
		}
		
		FWorldItemData* ItemData = ItemDataMap.Find(InUniqueID);

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, Location, Rotation, SpawnParameters);
		if (IsValid(SpawnedActor))
		{
			SpawnedActor->SetActorLocationAndRotation(ItemData->Location, ItemData->Rotation);
			// bool 변수 세팅 로직 필요
		}
	}
	else if (ActorCategory == EWorldActorCategory::Interactable)
	{
		if (InteractableDataMap.Find(InUniqueID) == nullptr)
		{
			FWorldInteractableData NewInteractableData;
			NewInteractableData.UniqueID = InUniqueID;
			NewInteractableData.ActorClass = ActorClass;
			NewInteractableData.bIsInteracted = false;
			NewInteractableData.Location = Location;
			NewInteractableData.Rotation = Rotation;

			InteractableDataMap.Add(InUniqueID, NewInteractableData);
		}
		
		FWorldInteractableData* InteractableData = InteractableDataMap.Find(InUniqueID);

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, Location, Rotation, SpawnParameters);
		if (IsValid(SpawnedActor))
		{
			SpawnedActor->SetActorLocationAndRotation(InteractableData->Location, InteractableData->Rotation);
			// bool 변수 세팅 로직 필요
		}
	}
	else if (ActorCategory == EWorldActorCategory::Enemy)
	{
		if (EnemyDataMap.Find(InUniqueID) == nullptr)
		{
			FWorldEnemyData NewEnemyData;
			NewEnemyData.UniqueID = InUniqueID;
			NewEnemyData.ActorClass = ActorClass;
			//NewEnemyData.CurrentHealth = ;
			NewEnemyData.Location = Location;
			NewEnemyData.Rotation = Rotation;

			EnemyDataMap.Add(InUniqueID, NewEnemyData);
		}
		
		FWorldEnemyData* EnemyData = EnemyDataMap.Find(InUniqueID);

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, Location, Rotation, SpawnParameters);
		if (IsValid(SpawnedActor))
		{
			SpawnedActor->SetActorLocationAndRotation(EnemyData->Location, EnemyData->Rotation);
		}
	}
}

void UWorldActorLoaderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UWorldActorLoaderSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UWorldActorLoaderSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	check(World);

	// WorldType이 Editor Preview같은 다른 타입인 경우에는 생성하지 않음
	if (World->WorldType != EWorldType::Game || World->WorldType != EWorldType::PIE)
	{
		return false;
	}

	// 이름 기준으로 오브젝트 풀링 필요없는 레벨에선 서브시스템 생성하지 않음
	const FString WorldName = World->GetName();
	if (WorldName.Contains("Title") || WorldName.Contains("MainMenu"))
	{
		return false;
	}

	return true;
}
