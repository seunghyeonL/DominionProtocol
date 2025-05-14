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
	//Setter
	UFUNCTION(BlueprintCallable)
	void SetMasterVolume(float NewVolume);
	UFUNCTION(BlueprintCallable)
	void SetSFXVolume(float NewVolume);
	UFUNCTION(BlueprintCallable)
	void SetBGMVolume(float NewVolume);
	UFUNCTION(BlueprintCallable)
	void SetUIVolume(float NewVolume);
	
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
