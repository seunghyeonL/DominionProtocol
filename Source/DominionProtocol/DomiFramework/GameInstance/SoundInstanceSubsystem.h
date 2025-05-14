// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumAndStruct/ECategory.h"
#include "EnumAndStruct/FSoundSubsystemData.h"
#include "SoundInstanceSubsystem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API USoundInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void LoadSoundClass();
	void LoadVolumeSettings();

	void OnLevelChanged(UWorld* NewWorld);

	//Save & Load
	void LoadSaveData(const FSoundSubsystemData& SaveData);
	FSoundSubsystemData GetSaveData() const;
	
	//Setter
	FORCEINLINE void SetWorldCache(UWorld* NewWorld) { World = NewWorld; }
	void SetMasterVolume(const float NewVolume);
	void SetSFXVolume(const float NewVolume);
	void SetBGMVolume(const float NewVolume);
	void SetUIVolume(const float NewVolume);
	
	FORCEINLINE void SetWorldCache(UWorld* NewWorld) { World = NewWorld; }
	
	//Getter
	FORCEINLINE float GetVolumeMultiplier(ESoundCategory Category) const { return CategoryVolumes[Category]; }
	
	void LoadSoundClass();
	void LoadVolumeSettings();

	void OnLevelChanged(UWorld* NewWorld);

	//Save & Load
	void LoadSaveData(const FSoundSubsystemData& SaveData);
	FSoundSubsystemData GetSaveData() const;

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
