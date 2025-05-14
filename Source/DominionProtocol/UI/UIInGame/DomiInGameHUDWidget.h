// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiInGameHUDWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiInGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void OnPlayerDeath();
	
	UFUNCTION()
	void OnPlayerSpawn();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowDeathScriptWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void HideDeathScriptWidget();

protected:
	virtual void NativeConstruct() override;
	void SetupStatusBarWidget(const AActor* OwningActor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiStatusBarWidget> StatusBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiAbilitySlotsWidget> AbilitySlotsWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiItemSlotsWidget> ItemSlotsWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiInGameMenuWidget> InGameMenuWidget;
};
