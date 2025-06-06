// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RenewalUI/NewSettingWidget.h"

#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"


void UNewSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const auto* GameInstance = GetWorld()->GetGameInstance();
	auto* SoundSubsystem = GameInstance->GetSubsystem<USoundInstanceSubsystem>();
	
	ensureMsgf(SoundSubsystem, TEXT("Casting failed to SoundSubsystem in OptionWidget"));
	
	if (SoundSubsystem)
	{
		SoundSubsystemInstance = SoundSubsystem;
	}
}

void UNewSettingWidget::SetMasterVolumeValue(const float NewVolumeValue)
{
	MasterVolumeValue = NewVolumeValue;
	SoundSubsystemInstance->SetMasterVolume(NewVolumeValue);
}

void UNewSettingWidget::SetBackgroundMusicVolumeValue(const float NewVolumeValue)
{
	BackgroundMusicVolumeValue = NewVolumeValue;
	SoundSubsystemInstance->SetBGMVolume(NewVolumeValue);
}

void UNewSettingWidget::SetSoundEffectsVolumeValue(const float NewVolumeValue)
{
	SoundEffectsVolumeValue = NewVolumeValue;
	SoundSubsystemInstance->SetSFXVolume(NewVolumeValue);
}

void UNewSettingWidget::SetUIEffectsVolumeValue(const float NewVolumeValue)
{
	UIEffectsVolumeValue = NewVolumeValue;
	SoundSubsystemInstance->SetUIVolume(NewVolumeValue);
}

