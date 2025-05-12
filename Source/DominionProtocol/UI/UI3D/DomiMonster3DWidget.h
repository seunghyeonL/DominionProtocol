// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DomiCharacter3DWidget.h"
#include "DomiMonster3DWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiMonster3DWidget : public UDomiCharacter3DWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateMonsterHPBar(const float NewHP);
	UFUNCTION(BlueprintCallable)
	void UpdateMonsterShield(const float NewShield);

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void SetOwningActor(AActor* NewOwner) override;
	void SetupMonster3dWidget();
	void SetWidgetRotation() const;
	

protected:
	// HP Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> MonsterHPBar;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentMonsterHP;

	UPROPERTY(BlueprintReadOnly)
	float PreMonsterHP;
	
	UPROPERTY(BlueprintReadOnly)
	float MaxMonsterHP;

	UPROPERTY(BlueprintReadWrite)
	float AlphaForAnimMonsterHP;

	// Shield Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> MonsterShieldBar;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentMonsterShield;

	UPROPERTY(BlueprintReadOnly)
	float PreMonsterShield;

	UPROPERTY(BlueprintReadOnly)
	float MaxMonsterShield;

	UPROPERTY(BlueprintReadWrite)
	float AlphaForAnimMonsterShield;

	UPROPERTY()
	TObjectPtr<class UDomiWidgetComponent> WidgetComponent;
};
