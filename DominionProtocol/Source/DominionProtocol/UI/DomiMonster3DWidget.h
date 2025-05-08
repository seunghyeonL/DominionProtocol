// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DomiCharacterWidget.h"
#include "DomiMonster3DWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiMonster3DWidget : public UDomiCharacterWidget
{
	GENERATED_BODY()

public:
	void UpdateMonsterHPBar(float NewHP);
	void UpdateMonsterShield(float NewShield);


protected:
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> MonsterHPBar;
	
	UPROPERTY()
	float CurrentMonsterHP;
	
	UPROPERTY()
	float MaxMonsterHP;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> MonsterShieldBar;
	
	UPROPERTY()
	float CurrentMonsterShield;

	UPROPERTY()
	float MaxMonsterShield;
};
