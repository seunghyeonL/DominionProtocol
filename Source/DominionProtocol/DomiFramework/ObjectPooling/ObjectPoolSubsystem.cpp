// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
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
