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

	UFUNCTION()
	void OnSwitchShowAndHideOnInGameMenuWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SwitchShowAndHideOnInGameMenuWidget();

	UFUNCTION(BlueprintCallable)
	void OnSwitchShowAndHideCrackWarpWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchShowAndHideCrackWarpWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchShowAndHideStatModifyWidget();

	UFUNCTION(BlueprintCallable)
	void ChangeWidgetZOrder(const UUserWidget* TargetWidget, const int32 NewZOrder);

protected:
	virtual void NativeConstruct() override;
	void SetupStatusBarWidget(const AActor* OwningActor);


protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AInGameController> OwningController;

	UPROPERTY(BlueprintReadWrite)
	bool bShowCrackWarpWidget = false;

#pragma region UI Alway
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiStatusBarWidget> StatusBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiItemSlotsWidget> ItemSlotsWidget;

#pragma endregion

#pragma region UI On/Off

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UUserWidget> InteractionWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UUserWidget> CrackWrapWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UBossMonsterHPBarWidget> BossMonsterHPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UStatModifyWidget> StatModifyWidget;
	
#pragma endregion
};
