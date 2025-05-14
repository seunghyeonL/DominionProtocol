// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "AudioMixerDevice.h"

#include "Util/DebugHelper.h"

void USoundInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CategoryVolumes.Add(ESoundCategory::Master, 1.f);
	CategoryVolumes.Add(ESoundCategory::SFX, 1.f);
	CategoryVolumes.Add(ESoundCategory::BGM, 1.f);
	CategoryVolumes.Add(ESoundCategory::UI, 1.f);
	
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &USoundInstanceSubsystem::OnLevelChanged);
}

void USoundInstanceSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

	Super::Deinitialize();
}

void USoundInstanceSubsystem::LoadSoundClass()
{
	check(World);
	
	VolumeControlMix = LoadObject<USoundMix>(nullptr, TEXT("/Game/Sound/Classes/SM_VolumeControl"));
	MasterClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sound/Classes/SC_Master"));
	SFXClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sound/Classes/SC_SFX"));
	BGMClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sound/Classes/SC_BGM"));
	UIClass = LoadObject<USoundClass>(nullptr, TEXT("/Game/Sound/Classes/SC_UI"));

	if (IsValid(VolumeControlMix))
	{
		UGameplayStatics::PushSoundMixModifier(World, VolumeControlMix);
	}
	
	Debug::PrintLog(TEXT("USoundInstanceSubsystem::LoadSoundClass : SoundClasses Loaded"));
}

void USoundInstanceSubsystem::LoadVolumeSettings()
{
	check(World);
	SetMasterVolume(CategoryVolumes[ESoundCategory::Master]);
	SetSFXVolume(CategoryVolumes[ESoundCategory::SFX]);
	SetBGMVolume(CategoryVolumes[ESoundCategory::BGM]);
	SetUIVolume(CategoryVolumes[ESoundCategory::UI]);
	Debug::PrintLog(TEXT("USoundInstanceSubsystem::LoadVolumeSettings called"));
}

void USoundInstanceSubsystem::OnLevelChanged(UWorld* NewWorld)
{
	World = NewWorld;
	LoadVolumeSettings();
}

void USoundInstanceSubsystem::LoadSaveData(const FSoundSubsystemData& SaveData)
{
	CategoryVolumes.Empty();
	CategoryVolumes = SaveData.CategoryVolumes;
}

FSoundSubsystemData USoundInstanceSubsystem::GetSaveData() const
{
	FSoundSubsystemData SaveData;

	SaveData.CategoryVolumes = CategoryVolumes;

	return SaveData;
}

void USoundInstanceSubsystem::SetMasterVolume(const float NewVolume)
{
	checkf(World, TEXT("World is not Loaded"));

	CategoryVolumes[ESoundCategory::Master] = NewVolume;

	if (IsValid(VolumeControlMix) && IsValid(MasterClass))
	{
		UGameplayStatics::SetSoundMixClassOverride(
			World,
			VolumeControlMix,
			MasterClass,
			CategoryVolumes[ESoundCategory::Master],
			1.0f,
			0.1f,
			true);
	}
}

void USoundInstanceSubsystem::SetSFXVolume(const float NewVolume)
{
	checkf(World, TEXT("World is not Loaded"));

	CategoryVolumes[ESoundCategory::SFX] = NewVolume;

	if (IsValid(VolumeControlMix) && IsValid(SFXClass))
	{
		UGameplayStatics::SetSoundMixClassOverride(
			World,
			VolumeControlMix,
			SFXClass,
			CategoryVolumes[ESoundCategory::SFX],
			1.0f,
			0.1f,
			false);
	}
}

void USoundInstanceSubsystem::SetBGMVolume(const float NewVolume)
{
	checkf(World, TEXT("World is not Loaded"));

	CategoryVolumes[ESoundCategory::BGM] = NewVolume;

	if (IsValid(VolumeControlMix) && IsValid(BGMClass))
	{
		UGameplayStatics::SetSoundMixClassOverride(
			World,
			VolumeControlMix,
			BGMClass,
			CategoryVolumes[ESoundCategory::BGM],
			1.0f,
			0.1f,
			false);
	}
}

void USoundInstanceSubsystem::SetUIVolume(const float NewVolume)
{
	checkf(World, TEXT("World is not Loaded"));

	CategoryVolumes[ESoundCategory::UI] = NewVolume;

	if (IsValid(VolumeControlMix) && IsValid(UIClass))
	{
		UGameplayStatics::SetSoundMixClassOverride(
			World,
			VolumeControlMix,
			UIClass,
			CategoryVolumes[ESoundCategory::UI],
			1.0f,
			0.1f,
			false);
	}
}