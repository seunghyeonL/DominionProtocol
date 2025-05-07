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

	VolumeControlMix = LoadObject<USoundMix>(nullptr, TEXT("Game/Sound/Classes/SM_VolumeControl"));
	MasterClass = LoadObject<USoundClass>(nullptr, TEXT("Game/Sound/SC_Master"));
	SFXClass = LoadObject<USoundClass>(nullptr, TEXT("Game/Sound/SC_SFX"));
	BGMClass = LoadObject<USoundClass>(nullptr, TEXT("Game/Sound/SC_BGM"));
	UIClass = LoadObject<USoundClass>(nullptr, TEXT("Game/Sound/SC_UI"));

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &USoundInstanceSubsystem::OnLevelChanged);
}

void USoundInstanceSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

	Super::Deinitialize();
}

void USoundInstanceSubsystem::LoadVolumeSettings()
{
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

void USoundInstanceSubsystem::SetMasterVolume(float NewVolume)
{
	checkf(World, TEXT("World is not Loaded"));

	NewVolume = FMath::Clamp(NewVolume, 0.0f, 100.0f);

	CategoryVolumes[ESoundCategory::Master] = NewVolume / 100.0f;

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

void USoundInstanceSubsystem::SetSFXVolume(float NewVolume)
{
	checkf(World, TEXT("World is not Loaded"));

	NewVolume = FMath::Clamp(NewVolume, 0.0f, 100.0f);

	CategoryVolumes[ESoundCategory::SFX] = NewVolume / 100.0f;

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

void USoundInstanceSubsystem::SetBGMVolume(float NewVolume)
{
	checkf(World, TEXT("World is not Loaded"));

	NewVolume = FMath::Clamp(NewVolume, 0.0f, 100.0f);

	CategoryVolumes[ESoundCategory::BGM] = NewVolume / 100.0f;

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

void USoundInstanceSubsystem::SetUIVolume(float NewVolume)
{
	checkf(World, TEXT("World is not Loaded"));

	NewVolume = FMath::Clamp(NewVolume, 0.0f, 100.0f);

	CategoryVolumes[ESoundCategory::UI] = NewVolume / 100.0f;

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
