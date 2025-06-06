// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewSettingWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UNewSettingWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetMasterVolumeValue(const float NewVolumeValue);

	UFUNCTION(BlueprintCallable)
	void SetBackgroundMusicVolumeValue(const float NewVolumeValue);

	UFUNCTION(BlueprintCallable)
	void SetSoundEffectsVolumeValue(const float NewVolumeValue);

	UFUNCTION(BlueprintCallable)
	void SetUIEffectsVolumeValue(const float NewVolumeValue);


protected:
	virtual void NativeConstruct() override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MasterVolumeValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float BackgroundMusicVolumeValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float SoundEffectsVolumeValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float UIEffectsVolumeValue;

	UPROPERTY()
	TObjectPtr<class USoundInstanceSubsystem> SoundSubsystemInstance;
	
};