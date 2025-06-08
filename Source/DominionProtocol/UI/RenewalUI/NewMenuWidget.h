// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewMenuWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UNewMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UDomiInGameHUDWidget> HUDWidget;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UUserWidget> OwnerWidget;
};
