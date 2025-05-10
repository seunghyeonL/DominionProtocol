// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ZoneBarrier.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerEnterZone);

UCLASS()
class DOMINIONPROTOCOL_API AZoneBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	AZoneBarrier();

	UPROPERTY(BlueprintAssignable)
	FOnPlayerEnterZone OnPlayerEnterZoneDelegate;

	void ActivateBarrier();
	void DeactivateBarrier();

	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* CollisionBox;

	UBoxComponent* Wall_Left;
	UBoxComponent* Wall_Right;
	UBoxComponent* Wall_Front;
	UBoxComponent* Wall_Back;
};
