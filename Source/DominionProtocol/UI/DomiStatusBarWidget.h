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


protected:
	void SetupHUDWidget();
	
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UProgressBar> PlayerHPBar;
	
	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UProgressBar> PlayerStaminaBar;

	UPROPERTY()
	float CurrentStamina;

	UPROPERTY()
	float MaxStamina;
	
};
