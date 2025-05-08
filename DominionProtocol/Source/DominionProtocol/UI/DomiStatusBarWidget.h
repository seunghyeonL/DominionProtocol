// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiStatusBarWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void UpdateHPBar();
	

protected:
	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
	
};
