// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncLoadBPLib.h"

#include "NiagaraFunctionLibrary.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"

void UAsyncLoadBPLib::AsyncSpawnNiagaraSystem(UObject* WorldContextObject, TSoftObjectPtr<UNiagaraSystem> SoftAsset, FVector SpawnLocation, FRotator SpawnRotation, FVector ScaleMultiplier)
{
	if (SoftAsset.ToSoftObjectPath().IsValid())
	{
		if (SoftAsset.IsValid())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				WorldContextObject,
				SoftAsset.Get(),
				SpawnLocation,
				SpawnRotation,
				ScaleMultiplier,
				true,
				true
			);
		}
		else
		{
			TWeakObjectPtr<UObject> WeakWorldContext = WorldContextObject;
			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
			Streamable.RequestAsyncLoad(SoftAsset.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([WeakWorldContext, SoftAsset, SpawnLocation, SpawnRotation, ScaleMultiplier]()
			{
				if (SoftAsset.IsValid() && WeakWorldContext.IsValid())
				{
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
					WeakWorldContext->GetWorld(),
					SoftAsset.Get(),
					SpawnLocation,
					SpawnRotation,
					ScaleMultiplier,
					true,
					true
					);	
				}
			}));
		}
	}
}

void UAsyncLoadBPLib::AsyncPlaySoundAtLocation(UObject* WorldContextObject, TSoftObjectPtr<USoundBase> SoftAsset, FVector SpawnLocation, float PitchMultiplier, float VolumeMultiplier)
{
	if (SoftAsset.ToSoftObjectPath().IsValid())
	{
		if (SoftAsset.IsValid())
		{
			UGameplayStatics::PlaySoundAtLocation(WorldContextObject, SoftAsset.Get(), SpawnLocation, FRotator(0.f), PitchMultiplier, VolumeMultiplier);
		}
		else
		{
			TWeakObjectPtr<UObject> WeakWorldContext = WorldContextObject;
			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
			Streamable.RequestAsyncLoad(SoftAsset.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([WeakWorldContext, SoftAsset, SpawnLocation, PitchMultiplier, VolumeMultiplier]()
			{
				if (SoftAsset.IsValid() && WeakWorldContext.IsValid())
				{
					UGameplayStatics::PlaySoundAtLocation(WeakWorldContext.Get(), SoftAsset.Get(), SpawnLocation, FRotator(0.f), PitchMultiplier, VolumeMultiplier);
				}
			}));
		}
	}
}

void UAsyncLoadBPLib::AsyncLoadAsset(TSoftObjectPtr<UObject> SoftAsset)
{
	FSoftObjectPath AssetPath = SoftAsset.ToSoftObjectPath();
	// When Path is not null.
	if (AssetPath.IsValid())
	{
		// When Asset is not loaded
		if (!SoftAsset.IsValid())
		{
			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
			Streamable.RequestAsyncLoad(SoftAsset.ToSoftObjectPath());
		}
	}
}

void UAsyncLoadBPLib::UnLoadAsset(TSoftObjectPtr<UObject> SoftAsset)
{
	FSoftObjectPath AssetPath = SoftAsset.ToSoftObjectPath();
	// When Path is not null.
	if (AssetPath.IsValid())
	{
		// When Asset is loaded
		if (SoftAsset.IsValid())
		{
			FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
			Streamable.Unload(AssetPath);
		}
	}
}
