// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorStateComponent.h"
#include "Util/GameTagList.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"

UActorStateComponent::UActorStateComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UActorStateComponent::SwitchStateAndUpdateInstance(FGameplayTag Tag, int32 StateLevel)
{
	if (Tag == WorldActorTags::ItemDropped)
	{
		ActorData.bIsItemCollected = true;
	}
	else if (Tag == WorldActorTags::OpenableChestItem)
	{
		if (StateLevel == 0)
		{
			ActorData.bIsChestOpened = true;
		}
		else if (StateLevel == 1)
		{
			ActorData.bIsItemCollected = true;
		}
	}
	else if (Tag == WorldActorTags::ItemContainer)
	{
		ActorData.bIsItemCollected = true;
	}
	else if (Tag == WorldActorTags::DyingHelper)
	{
		ActorData.bIsDead = true;
		if (ActorData.bIsDead)
		{
			OnDyingHelperDead.Broadcast(ActorData.bIsDead);
		}
	}

	WorldInstanceSubsystem->SetWorldActorData(UniqueActorID, ActorData);
}

void UActorStateComponent::BeginPlay()
{
	Super::BeginPlay();

	WorldInstanceSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
	check(IsValid(WorldInstanceSubsystem));

	// 이미 인스턴스 상에 데이터 존재하면 불러오기
	if (WorldInstanceSubsystem->GetIsWorldActorDataExist(UniqueActorID))
	{
		ActorData = WorldInstanceSubsystem->GetWorldActorData(UniqueActorID);
		if (ActorData.UniqueActorID == UniqueActorID)
		{
			UniqueActorID = ActorData.UniqueActorID;
		}
	}
	// 인스턴스 상에 존재하지 않으면 데이터 초기화
	else
	{
		ActorData.UniqueActorID = UniqueActorID;

		WorldInstanceSubsystem->SetWorldActorData(UniqueActorID, ActorData);
	}
}

#if WITH_EDITOR
void UActorStateComponent::OnComponentCreated()
{
	Super::OnComponentCreated();

	if (!UniqueActorID.IsValid())
	{
		UniqueActorID = FGuid::NewGuid();
		Modify();
	}
}
#endif