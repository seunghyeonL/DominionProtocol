// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiAbilitySlotsWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiAbilitySlotsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void OnUseWeaponSkill();

	UFUNCTION()
	void OnUseMagicSkill();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWeaponSkillIcon();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMagicSkillIcon();

protected:
	virtual void NativeConstruct()override;

	void BindUseWeaponSkillEvent();
	void BindUseMagicSkillEvent();
};
