// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorStateManageWorldSubsystem.h"

#include "Util/DebugHelper.h"

void UActorStateManageWorldSubsystem::UpdateActorDataMap(FGuid ID, FWorldActorData& Data)
{
	if (!ActorDataMap.Contains(ID))
	{
		Debug::PrintError(TEXT("UActorStateManageWorldSubsystem::UpdateActorDataMap : ActorDataMap does not exist"));
		return;
	}

	ActorDataMap[ID] = Data;
}

void UActorStateManageWorldSubsystem::UpdateWorldInstanceActorDataMap()
{
	if (IsValid(WorldInstanceSubsystem))
	{
		WorldInstanceSubsystem->SetWorldActorDataMap(ActorDataMap);
	}
}

void UActorStateManageWorldSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	WorldInstanceSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
	check(IsValid(WorldInstanceSubsystem));
}

void UActorStateManageWorldSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UActorStateManageWorldSubsystem::ShouldCreateSubsystem(UObject* Outer) const
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
