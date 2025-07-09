// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "TitleController.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnStartGame)
DECLARE_MULTICAST_DELEGATE(FOnDeleteGame)
DECLARE_MULTICAST_DELEGATE(FOnBackToTitleMenu)
DECLARE_MULTICAST_DELEGATE(FOnMoveSelectionUp)
DECLARE_MULTICAST_DELEGATE(FOnMoveSelectionDown)
DECLARE_MULTICAST_DELEGATE(FOnConfirmSelection)

class UNewTitleHUDWidget;

UCLASS()
class DOMINIONPROTOCOL_API ATitleController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	FOnStartGame OnStartGame;
	FOnDeleteGame OnDeleteGame;
	FOnBackToTitleMenu OnBackToTitleMenu;
	FOnMoveSelectionUp OnMoveSelectionUp;
	FOnMoveSelectionDown OnMoveSelectionDown;
	FOnConfirmSelection OnConfirmSelection;
	
	ATitleController();

	// Bind Action
	UFUNCTION()
	void OnTitleSlotUIStartGame() const;

	UFUNCTION()
	void OnTitleSlotUIDeleteGame() const;

	UFUNCTION()
	void OnTitleSlotUIBackToTitleMenu() const;

	UFUNCTION()
	void OnTitleMenuUIMoveSelectionUp() const;

	UFUNCTION()
	void OnTitleMenuUIMoveSelectionDown() const;

	UFUNCTION()
	void OnTitleMenuUIConfirmSelection() const;
	
	void SetupMappingContext(const UInputMappingContext* NewInputMappingContext);

	FORCEINLINE float GetFadeDuration() const { return FadeDuration; }
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void CreateAndAddHUDWidget() override;
	
	UFUNCTION()
	virtual void SetupMappingContext() override;
	
	UFUNCTION()
	virtual void SetupInputMode() override;

	UFUNCTION()
	virtual void BindInputActions() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> TitleSlotUIMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> TitleMenuUIMappingContext;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UNewTitleHUDWidget> TitleHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UNewTitleHUDWidget> TitleHUDWidgetInstance;
	
#pragma region TitleMenuUI Input Actions Section

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputTitleMenuUIMoveSelectionUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputTitleMenuUIMoveSelectionDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputTitleMenuUIConfirmSelection;


#pragma endregion

#pragma region SlotUI Input Actions Section
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputTitleSlotUIStartGame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputTitleSlotUIDeleteGame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputTitleSlotUIBackToTitleMenu;
	
#pragma endregion
};
