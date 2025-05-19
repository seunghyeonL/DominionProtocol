// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWorldActorData.generated.h"

USTRUCT(BlueprintType)
struct FWorldItemData
{
	GENERATED_BODY()

	// 고유식별자(각 객체별)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid UniqueID;

	// 액터 클래스 저장
	UPROPERTY()
	TSubclassOf<AActor> ActorClass;
	
	// 아이템인 경우 습득 여부
	UPROPERTY()
	bool bIsCollected;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
};

USTRUCT(BlueprintType)
struct FWorldInteractableData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid UniqueID;

	UPROPERTY()
	TSubclassOf<AActor> ActorClass;
	
	// 상호작용 대상(문, 아이템 상자, 엘리베이터 등등...) 사용 여부
	UPROPERTY()
	bool bIsInteracted;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
};

USTRUCT(BlueprintType)
struct FWorldEnemyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGuid UniqueID;

	UPROPERTY()
	TSubclassOf<AActor> ActorClass;
	
	// 적의 경우 체력 정보
	UPROPERTY()
	float CurrentHealth;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator Rotation;
};