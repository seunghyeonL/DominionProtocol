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
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerHPBar(const float NewHP);
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerStaminaBar(const float NewStamina);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AnimUpdatePlayerHPBar();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AnimUpdatePlayerStaminaBar();


protected:
	void SetupStatusBarWidget(const AActor* OwningActor);

	UFUNCTION(BlueprintCallable)
	virtual void NativeConstruct() override;
	

protected:
	// HP Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<class UProgressBar> PlayerHPBar;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentHP;

	UPROPERTY(BlueprintReadOnly)
	float PreHP;

	UPROPERTY(BlueprintReadOnly)
	float MaxHP;

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
	
	UPROPERTY(BlueprintReadWrite)
	float AlphaForStaminaAnim;
};
