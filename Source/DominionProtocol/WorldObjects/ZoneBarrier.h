// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ZoneBarrier.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API AZoneBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	AZoneBarrier();

	void ActivateBarrier();
	void DeactivateBarrier();

	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Wall_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Wall_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Wall_Front;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Wall_Back;
};
