// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "BossTrigger.generated.h"

class UBoxComponent;
class ABossSpawner;

UCLASS()
class DOMINIONPROTOCOL_API ABossTrigger : public AActor
{
	GENERATED_BODY()
	
public:
	ABossTrigger();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerBox;

public:
	UPROPERTY(EditAnywhere)
	ABossSpawner* LinkedBossSpawner;

private:
	UPROPERTY()
	bool bHasStoryUpdated;

	UPROPERTY(EditAnywhere)
	EGameStoryState StoryStateToSet;

};
