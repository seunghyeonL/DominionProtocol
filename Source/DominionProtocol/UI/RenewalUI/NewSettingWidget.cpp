// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RenewalUI/NewSettingWidget.h"

#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"


void UNewSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto* GameInstance = GetWorld()->GetGameInstance();
	auto* SoundSubsystem = GameInstance->GetSubsystem<USoundInstanceSubsystem>();
	
	ensureMsgf(SoundSubsystem, TEXT("Casting failed to SoundSubsystem in UNewSettingWidget"));
	
	if (SoundSubsystem)
	{
		SoundSubsystemInstance = SoundSubsystem;
	}
}

void UNewSettingWidget::ApplyMasterVolumeValue(const float NewVolumeValue)
{
	if (SoundSubsystemInstance)
	{
		MasterVolumeValue = NewVolumeValue;
		SoundSubsystemInstance->SetMasterVolume(NewVolumeValue);
	}
}

void UNewSettingWidget::ApplyBackgroundMusicVolumeValue(const float NewVolumeValue)
{
	if (SoundSubsystemInstance)
	{
		BackgroundMusicVolumeValue = NewVolumeValue;
		SoundSubsystemInstance->SetBGMVolume(NewVolumeValue);
	}
}

void UNewSettingWidget::ApplySoundEffectsVolumeValue(const float NewVolumeValue)
{
	if (SoundSubsystemInstance)
	{
		SoundEffectsVolumeValue = NewVolumeValue;
		SoundSubsystemInstance->SetSFXVolume(NewVolumeValue);
	}
}

void UNewSettingWidget::ApplyUIEffectsVolumeValue(const float NewVolumeValue)
{
	if (SoundSubsystemInstance)
	{
		UIEffectsVolumeValue = NewVolumeValue;
		SoundSubsystemInstance->SetUIVolume(NewVolumeValue);
	}
}

void UNewSettingWidget::SetMasterVolumeValue(const float NewVolumeValue)
{
	if (SoundSubsystemInstance)
	{
		DesiredMasterVolumeValue = NewVolumeValue;
		SoundSubsystemInstance->SetMasterVolume(NewVolumeValue);
	}
}

void UNewSettingWidget::SetBackgroundMusicVolumeValue(const float NewVolumeValue)
{
	if (SoundSubsystemInstance)
	{
		DesiredBackgroundMusicVolumeValue = NewVolumeValue;
		SoundSubsystemInstance->SetBGMVolume(NewVolumeValue);
	}
}

void UNewSettingWidget::SetSoundEffectsVolumeValue(const float NewVolumeValue)
{
	if (SoundSubsystemInstance)
	{
		DesiredSoundEffectsVolumeValue = NewVolumeValue;
		SoundSubsystemInstance->SetSFXVolume(NewVolumeValue);
	}
}

void UNewSettingWidget::SetUIEffectsVolumeValue(const float NewVolumeValue)
{
	if (SoundSubsystemInstance)
	{
		DesiredUIEffectsVolumeValue = NewVolumeValue;
		SoundSubsystemInstance->SetUIVolume(NewVolumeValue);
	}
}

