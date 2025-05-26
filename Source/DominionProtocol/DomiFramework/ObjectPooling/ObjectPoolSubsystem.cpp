// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"
#include "DomiFramework/ObjectPooling/PoolableBlueprintClassDataAsset.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	PoolConfig = LoadObject<UPoolableBlueprintClassDataAsset>(nullptr, TEXT("/Game/Data/DataAsset/DA_PoolableBlueprintClass"));

	if(!IsValid(PoolConfig))
	{
		Debug::PrintError(TEXT("Failed to load DataAsset : DA_PoolableBlueprintClass"));
	}
	else
	{
		Debug::PrintLog(TEXT("DA_PoolableBlueprintClass Loaded"));
	}

	//오브젝트풀링 시스템 TMap에 데이터에셋의 블루프린트 클래스 맵핑(태그를 Key로)
	for (const FPoolableActorBlueprintData& Data : PoolConfig->PoolableBlueprintClassArray)
	{
		if (IsValid(Data.ActorClass) && Data.Tag.IsValid())
		{
			TagToClassMap.Add(Data.Tag, Data.ActorClass);
		}
	}
}

void UObjectPoolSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UObjectPoolSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	check(World);

	// WorldType이 Editor Preview같은 다른 타입인 경우에는 생성하지 않음
	if (World->WorldType != EWorldType::Game && World->WorldType != EWorldType::PIE)
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

AActor* UObjectPoolSubsystem::SpawnActorFromPool(const FGameplayTag& Tag, const FVector& SpawnLocation,
	const FRotator& SpawnRotation, APawn* Instigator)
{
	if (!TagToClassMap.Contains(Tag))
	{
		Debug::PrintError(TEXT("UObjectPoolSubsystem::SpawnActorFromPool : Tag not Founded"));
		return nullptr;
	}

	UClass* ClassType = TagToClassMap[Tag].Get();
	if (!IsValid(ClassType))
	{
		Debug::PrintError(TEXT("UObjectPoolSubsystem::SpawnActorFromPool : Class for Tag is null"));
		return nullptr;
	}

	return SpawnActorFromPoolByClass(ClassType, SpawnLocation, SpawnRotation, Instigator);
}

AActor* UObjectPoolSubsystem::SpawnActorFromPoolByClass(UClass* ClassType, const FVector& SpawnLocation,
	const FRotator& SpawnRotation, APawn* Instigator)
{
	if (!ClassType)
	{
		Debug::PrintError(TEXT("UObjectPoolSubsystem::SpawnActorFromPool : Invalid ClassType"));
		return nullptr;
	}

	// 해당 클래스가 이미 풀에 존재하는지 확인
	FPooledActorList* PoolList = PooledActorsMap.Find(ClassType);
	if (!PoolList)
	{
		FPooledActorList NewList;
		PooledActorsMap.Add(ClassType, NewList);
		PoolList = PooledActorsMap.Find(ClassType);
	}

	//재사용
	if (PoolList)
	{
		for (TObjectPtr<AActor>& PooledActor : PoolList->PooledActors)
		{
			if (PooledActor->IsHidden())
			{
				PooledActor->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
				PooledActor->SetInstigator(Instigator);
				PooledActor->SetOwner(Instigator);

				if (PooledActor->Implements<UPoolableObjectInterface>())
				{
					IPoolableObjectInterface::Execute_OnObjectSpawn(PooledActor);
				}
				return PooledActor.Get();
			}
		}
	}

	//없을 시, 생성
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		UE_LOG(LogTemp, Warning, TEXT("UObjectPoolSubsystem::SpawnActorFromPool : Invalid World"));
		return nullptr;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AActor* NewActor = World->SpawnActor<AActor>(ClassType, SpawnLocation, SpawnRotation, SpawnParams);
	if (!NewActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnActorFromPoolByClass: Failed to spawn actor"));
		return nullptr;
	}

	if (NewActor->Implements<UPoolableObjectInterface>())
	{
		NewActor->SetInstigator(Instigator);
		NewActor->SetOwner(Instigator);

		PoolList->PooledActors.Add(NewActor);

		IPoolableObjectInterface::Execute_OnObjectSpawn(NewActor);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnActorFromPoolByClass: NewActor does not implement PoolableObjectInterface"));
		NewActor->Destroy();
		return nullptr;
	}
	
	return nullptr;
}

void UObjectPoolSubsystem::ClearObjectPool()
{
	{
		for (auto& Elem : PooledActorsMap)
		{
			for (TObjectPtr<AActor>& Actor : Elem.Value.PooledActors)
			{
				if (IsValid(Actor))
				{
					Actor->Destroy();
				}
			}
		}
		PooledActorsMap.Empty();
	}
}

void UObjectPoolSubsystem::ReturnActorToPool(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		Debug::Print("UObjectPoolSubsystem::ReturnActorToPool: Actor is null");
		return;
	}
	else if (Actor->Implements<UPoolableObjectInterface>())
	{
		IPoolableObjectInterface::Execute_OnObjectReturn(Actor);
	}
}
