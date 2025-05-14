// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UITitle/DomiOptionWidget.h"

#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"


void UDomiOptionWidget::NativeConstruct()
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

void UDomiOptionWidget::SetMasterVolumeValue(const float NewVolumeValue)
{
	MasterVolumeValue = NewVolumeValue;
	SoundSubsystemInstance->SetMasterVolume(MasterVolumeValue);
}

void UDomiOptionWidget::SetBackgroundMusicVolumeValue(const float NewVolumeValue)
{
	BackgroundMusicVolumeValue = NewVolumeValue;
	SoundSubsystemInstance->SetBGMVolume(NewVolumeValue);
}

void UDomiOptionWidget::SetSoundEffectsVolumeValue(const float NewVolumeValue)
{
	SoundEffectsVolumeValue = NewVolumeValue;
	SoundSubsystemInstance->SetSFXVolume(NewVolumeValue);
}

void UDomiOptionWidget::SetUIEffectsVolumeValue(const float NewVolumeValue)
{
	UIEffectsVolumeValue = NewVolumeValue;
	SoundSubsystemInstance->SetUIVolume(NewVolumeValue);
}

