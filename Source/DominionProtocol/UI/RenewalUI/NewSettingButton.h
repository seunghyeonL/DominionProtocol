// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewSettingButton.generated.h"

UENUM(BlueprintType)
enum class ESettingButtonType : uint8
{
	EGraphicSettingButton			UMETA(DisplayName="GraphicSettingButton"),
	ESoundSettingButton			UMETA(DisplayName="SoundSettingButton"),
	EExitButton					UMETA(DisplayName="ExitButton"),
	EDefault					UMETA(DisplayName="Default"),
};

UCLASS()
class DOMINIONPROTOCOL_API UNewSettingButton : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESettingButtonType SettingButtonType = ESettingButtonType::EDefault;
};
