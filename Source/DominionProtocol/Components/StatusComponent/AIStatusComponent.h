// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "AIStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOMINIONPROTOCOL_API UAIStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIStatusComponent();

	const FGameplayTagContainer& GetActiveTags() const { return ActiveTags; }

	void AddTag(const FGameplayTag& Tag);
	void RemoveTag(const FGameplayTag& Tag);
	void SetExclusiveState(const FGameplayTag& NewState);
protected:
	FGameplayTagContainer ActiveTags;
};
