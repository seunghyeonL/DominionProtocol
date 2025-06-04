// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnumAndStruct/FWorldActorData.h"
#include "Util/GameTagList.h"
#include "ActorStateComponent.generated.h"


class UWorldInstanceSubsystem;
class UActorStateManageWorldSubsystem;
class UActorStateData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API UActorStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActorStateComponent();

	void SwitchStateAndUpdateInstance(FGameplayTag Tag, int32 StateLevel = 0);
	
	// Getter
	FORCEINLINE const FGuid& GetUniqueActorID() const { return UniqueActorID; }
	FORCEINLINE const FWorldActorData& GetActorData() const { return ActorData; }

	// Setter
	void SetGameplayTag(FGameplayTag Tag) { ActorData.Tag = Tag; }
	// void SetActorDataAndUpdateInstance(FWorldActorData NewActorData);
	
protected:
	virtual void BeginPlay() override;
#if WITH_EDITOR
	virtual void OnComponentCreated() override;
#endif

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Property | UniqueActorID")
	FGuid UniqueActorID;

private:
	UPROPERTY()
	FWorldActorData ActorData;

	UPROPERTY()
	TObjectPtr<UWorldInstanceSubsystem> WorldInstanceSubsystem;

	UPROPERTY()
	TObjectPtr<UActorStateManageWorldSubsystem> StateWorldSubsystem;
};
