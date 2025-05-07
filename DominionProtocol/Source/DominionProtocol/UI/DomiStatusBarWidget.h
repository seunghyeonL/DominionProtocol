// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DomiUserWidget.h"
#include "DomiStatusBarWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiStatusBarWidget : public UDomiUserWidget
{
	GENERATED_BODY()


protected:

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
