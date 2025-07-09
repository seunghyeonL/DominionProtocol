// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewShotcutButton.generated.h"

UENUM(BlueprintType)
enum class EShotcutButtonType : uint8
{
	ESelectLeftButton			UMETA(DisplayName="SelectLeftButton"),
	ESelectRightButton			UMETA(DisplayName="SelectRightButton"),
	EBackButton					UMETA(DisplayName="BackButton"),
	EDefault					UMETA(DisplayName="Default"),
};


UCLASS()
class DOMINIONPROTOCOL_API UNewShotcutButton : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EShotcutButtonType ShotcutButtonType = EShotcutButtonType::EDefault;
};
