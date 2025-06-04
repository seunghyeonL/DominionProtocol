// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnumAndStruct/EffectData/EffectUIData.h"
#include "DomiStatusBarWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiStatusBarWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void UpdatePlayerHPBar(const float NewHP);
	UFUNCTION()
	void UpdatePlayerMaxHPBar(const float NewMaxHP);
	UFUNCTION()
	void UpdatePlayerStaminaBar(const float NewStamina);
	UFUNCTION()
	void UpdatePlayerMaxStaminaBar(const float NewMaxStamina);
	UFUNCTION()
	void OnUpdateEffectUIDataArray(TArray<FEffectUIData> NewEffectUIDataArray);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEffectUIDataArray();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AnimUpdatePlayerHPBar();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AnimUpdatePlayerStaminaBar();


protected:
	void SetupStatusBarWidget(AActor* OwningActor);

	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	

protected:
	UPROPERTY()
	TObjectPtr<class UStatusComponent> StatusComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<FEffectUIData> EffectUIDataArray;
	
	// HP Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UProgressBar> PlayerHPBar;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentHP;

	UPROPERTY(BlueprintReadOnly)
	float PreHP;

	UPROPERTY(BlueprintReadOnly)
	float MaxHP;

	UPROPERTY(BlueprintReadOnly)
	float PreMaxHP;

	UPROPERTY(BlueprintReadWrite)
	float AlphaForHPAnim;


	// Stamina Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UProgressBar> PlayerStaminaBar;

	UPROPERTY(BlueprintReadOnly)
	float CurrentStamina;

	UPROPERTY(BlueprintReadOnly)
	float PreStamina;

	UPROPERTY(BlueprintReadOnly)
	float MaxStamina;

	UPROPERTY(BlueprintReadOnly)
	float PreMaxStamina;	
	
	UPROPERTY(BlueprintReadWrite)
	float AlphaForStaminaAnim;
};
