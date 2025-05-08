// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DomiCharacter3DWidget.h"
#include "DomiMonster3DWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiMonster3DWidget : public UDomiCharacter3DWidget
{
	GENERATED_BODY()

public:
	void UpdateMonsterHPBar(const float NewHP);
	void UpdateMonsterShield(const float NewShield);


protected:
	virtual void NativeConstruct() override;

	virtual void SetOwningActor(AActor* NewOwner) override;
	void SetupMonster3dWidget();
	

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
