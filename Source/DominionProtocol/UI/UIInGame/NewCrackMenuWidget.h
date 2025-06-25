// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewCrackMenuWidget.generated.h"

UENUM(BlueprintType)
enum class ECurrentDisplayOption : uint8
{
	CrackTeleport			UMETA(DisplayName="CrackTeleport"),
	AbilityEnforce			UMETA(DisplayName="AbilityEnforce"),
	PotionEnforce			UMETA(DisplayName="PotionEnforce"),
	Default					UMETA(DisplayName="Default")
};

UCLASS()
class DOMINIONPROTOCOL_API UNewCrackMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnPressedBackButton();

	UFUNCTION()
	void OnPressedConfirmButton();

	UFUNCTION(BlueprintImplementableEvent)
	void PressedBackButton();

	UFUNCTION(BlueprintImplementableEvent)
	void PressedConfirmButton();

	
protected:
	virtual void NativeConstruct() override;

	void BindInputActionDelegates();

protected:
	UPROPERTY(BlueprintReadWrite)
	ECurrentDisplayOption CurrentDisplayOption = ECurrentDisplayOption::Default;
};
