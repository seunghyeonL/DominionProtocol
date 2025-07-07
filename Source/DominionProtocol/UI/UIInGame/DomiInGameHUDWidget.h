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
	void OnShowCrackMenuWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowCrackMenuWidget();

	// Binding Input Action
	UFUNCTION()
	void OnMainMenuSwitchShowAndHideWidget();

	UFUNCTION(BlueprintImplementableEvent)
	void MainMenuSwitchShowAndHideWidget();

	UFUNCTION()
	void OnDialogueChangedNextStoryState();

	UFUNCTION(BlueprintImplementableEvent)
	void DialogueChangedNextStoryState();

	//
	UFUNCTION(BlueprintCallable)
	void ChangeWidgetZOrder(const UUserWidget* TargetWidget, const int32 NewZOrder);

protected:
	virtual void NativeConstruct() override;

	void BindInputActionDelegates();


protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class AInGameController> OwningController;

#pragma region UI Alway
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiStatusBarWidget> StatusBarWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UDomiItemSlotsWidget> ItemSlotsWidget;

#pragma endregion

#pragma region UI On/Off
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UAlertWidget> AlertWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UNewEventBoxWidget> EventBoxWidget; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UUserWidget> InteractionWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UUserWidget> CrackMenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UBossMonsterHPBarWidget> BossMonsterHPBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UUserWidget> MainMenuWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UNewDialogueWidget> DialogueWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<class UUserWidget> LockOnHUDWidget;

	
	
#pragma endregion
};
