// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossMonsterHPBarWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UBossMonsterHPBarWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void SetActiveBossBattle(const bool NewValue);
	
	void UpdateBossMonsterHP(const float NewHP);
	void UpdateBossMonsterShield(const float NewShield);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CreateBossMonsterHPBarWidget();

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bActiveBossBattle;

	// HP Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentBossMonsterHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PreBossMonsterHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxBossMonsterHP;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AlphaForBossMonsterHPAnim;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> BossMonsterHPBar;
	

	// Shield Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentBossMonsterShield;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PreBossMonsterShield;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float MaxBossMonsterShield;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float AlphaForBossMonsterShieldAnim;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UProgressBar> BossMonsterShieldBar;
};
