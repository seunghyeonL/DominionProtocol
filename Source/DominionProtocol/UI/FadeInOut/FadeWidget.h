// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadeWidget.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UFadeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn(float PlayTime);

	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut(float PlayTime);
};
