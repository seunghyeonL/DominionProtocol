// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Interface/PoolableObjectInterface.h"
#include "Util/DebugHelper.h"
#include "ObjectPoolSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FPooledActorList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<AActor>> PooledActors;
};

UCLASS()
class DOMINIONPROTOCOL_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void ClearObjectPool();
	
	void ReturnActorToPool(AActor* Actor);
	
	template <typename T>
	T* SpawnActorFromPool(const FVector& SpawnLocation, const FRotator& SpawnRotation, APawn* Instigator = nullptr)
	{
		static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T must be AActor");

		UClass* ClassType = T::StaticClass();

		// 해당 클래스(Key)가 TMap에 없으면 새로 생성
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
					return Cast<T>(PooledActor);
				}
			}
		}

		//없을 시, 생성
		UWorld* World = GetWorld();
		if (!IsValid(World))
		{
			return nullptr;
		}

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		T* NewActor = World->SpawnActor<T>(ClassType, SpawnLocation, SpawnRotation, SpawnParams);
		if (IPoolableObjectInterface* PoolableObject = Cast<IPoolableObjectInterface>(NewActor))
		{
			NewActor->SetInstigator(Instigator);
			NewActor->SetOwner(Instigator);
			PoolList->PooledActors.Add(NewActor);
			IPoolableObjectInterface::Execute_OnObjectSpawn(NewActor);
		}
		else
		{
			Debug::Print("ObjectPoolSubsystem::SpawnPooledActor : NewActor is not a PoolableObject");
			NewActor->Destroy();
			return nullptr;
		}

		return Cast<T>(NewActor);
	}

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	
private:
	UPROPERTY()
	TMap<UClass*, FPooledActorList> PooledActorsMap;
};
