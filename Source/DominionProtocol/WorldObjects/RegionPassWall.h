// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RegionPassWall.generated.h"

class UArrowComponent;
class UWorldActorLoaderSubsystem;
class UBoxComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionChanged, int32, NewValue);

UCLASS()
class DOMINIONPROTOCOL_API ARegionPassWall : public AActor
{
	GENERATED_BODY()

// Functions
public:
	ARegionPassWall();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

// Variables
public:
	UPROPERTY(BlueprintAssignable)
	FOnRegionChanged OnRegionChanged;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UArrowComponent* ArrowComponent;
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 NextRegionIndex;
	
private:
	UPROPERTY()
	UWorldActorLoaderSubsystem* WorldActorLoaderSubsystem;

	int32 PreviousRegionIndex;
};
