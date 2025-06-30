// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewMainMenuWidget.generated.h"


UENUM(BlueprintType)
enum class EDisplayMenu : uint8
{
	StatusMenu			UMETA(DisplayName="StatusMenu"),
	EquipmentMenu		UMETA(DisplayName="EquipmentMenu"),
	ItemMenu			UMETA(DisplayName="ItemMenu"),
	SettingMenu			UMETA(DisplayName="SettingMenu"),
	Default				UMETA(DisplayName="Default")
};

UCLASS()
class DOMINIONPROTOCOL_API UNewMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

	void BindInputActionDelegates();
	
	UFUNCTION(BlueprintCallable)
	void ChangeWidgetZOrder(const UUserWidget* TargetWidget, const int32 NewZOrder);

	UFUNCTION()
	void OnPressButtonZ();

	UFUNCTION()
	void OnPressButtonC();

	UFUNCTION()
	void OnPressButtonA();

	UFUNCTION()
	void OnPressButtonD();

	UFUNCTION()
	void OnPressButtonE();

	UFUNCTION()
	void OnPressButtonR();

	UFUNCTION()
	void OnPressButtonSpaceBar();

	UFUNCTION()
	void OnPressButtonQ();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonZ();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonC();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonA();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonD();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonE();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonR();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonSpaceBar();

	UFUNCTION(BlueprintImplementableEvent)
	void PressButtonQ();

protected:
	UPROPERTY(BlueprintReadWrite)
	EDisplayMenu CurrentDisplayMenu = EDisplayMenu::Default;
	
};
