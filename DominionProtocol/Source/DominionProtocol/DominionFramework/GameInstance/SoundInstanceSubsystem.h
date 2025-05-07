// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundInstanceSubsystem.generated.h"

UENUM(BlueprintType)
enum class ESoundCategory : uint8
{
	Master UMETA(DisplayName = "Master"),
	SFX UMETA(DisplayName = "SFX"),
	BGM UMETA(DisplayName = "BGM"),
	UI UMETA(DisplayName = "UI")
};

UCLASS()
class DOMINIONPROTOCOL_API USoundInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void LoadVolumeSettings();

	void OnLevelChanged(UWorld* NewWorld);
	
	//Setter
	void SetMasterVolume(float NewVolume);
	void SetSFXVolume(float NewVolume);
	void SetBGMVolume(float NewVolume);
	void SetUIVolume(float NewVolume);
	//Getter
	FORCEINLINE float GetVolumeMultiplier(ESoundCategory Category) const { return CategoryVolumes[Category]; }

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:
	UPROPERTY()
	TMap<ESoundCategory, float> CategoryVolumes;

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	USoundMix* VolumeControlMix;

	UPROPERTY()
	USoundClass* MasterClass;

	UPROPERTY()
	USoundClass* SFXClass;

	UPROPERTY()
	USoundClass* BGMClass;

	UPROPERTY()
	USoundClass* UIClass;
};
