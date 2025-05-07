// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Crack.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API ACrack : public AActor
{
	GENERATED_BODY()
	
public:	
	ACrack();
	void MoveToLevel();
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp, 
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult& SweepResult);

public:	
	bool bIsActivate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Scene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* Collision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;
};
