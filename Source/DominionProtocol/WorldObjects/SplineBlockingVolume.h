// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineBlockingVolume.generated.h"

class USplineComponent;
class UBoxComponent;

UCLASS()
class DOMINIONPROTOCOL_API ASplineBlockingVolume : public AActor
{
	GENERATED_BODY()

public:
	ASplineBlockingVolume();

protected:
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blocking Volume")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking Volume")
	TObjectPtr<USplineComponent> SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking Volume")
	float MaxSpacing = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking Volume")
	float MinSpacing = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking Volume")
	float CurvatureMax = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blocking Volume")
	FVector CollisionBoxExtent = FVector(50.f, 10.f, 400.f);

private:
	UPROPERTY()
	TArray<UBoxComponent*> BoxComponents;

	void ClearBox();
	void CreateBox();
	float CalculateCurvatureAtDistance(float Distance) const;
};
