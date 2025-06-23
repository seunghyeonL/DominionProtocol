// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AsyncLoadBPLib.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAsyncLoadBPLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void AsyncSpawnNiagaraSystem(
		UObject* WorldContextObject,
		TSoftObjectPtr<UNiagaraSystem> SoftAsset,
		FVector SpawnLocation,
		FRotator SpawnRotation = FRotator(0.f, 0.f, 0.f),
		FVector ScaleMultiplier = FVector(1.f)
		);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static void AsyncPlaySoundAtLocation(
		UObject* WorldContextObject,
		TSoftObjectPtr<USoundBase> SoftAsset,
		FVector SpawnLocation,
		float PitchMultiplier = 1.f,
		float VolumeMultiplier = 1.f
		);

	UFUNCTION(BlueprintCallable)
	static void AsyncLoadAsset(TSoftObjectPtr<UObject> SoftAsset);
	
	UFUNCTION(BlueprintCallable)
	static void UnLoadAsset(TSoftObjectPtr<UObject> SoftAsset);
};
