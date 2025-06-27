// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitleController.generated.h"

DECLARE_DELEGATE(FOnPressedStartGame)
DECLARE_DELEGATE(FOnPressedDeleteGame)
DECLARE_DELEGATE(FOnPressedBackToMainMenu)

UCLASS()
class DOMINIONPROTOCOL_API ATitleController : public APlayerController
{
	GENERATED_BODY()

public:
	FOnPressedStartGame OnPressedStartGame;
	FOnPressedDeleteGame OnPressedDeleteGame;
	FOnPressedBackToMainMenu OnPressedBackToMainMenu;
	
	ATitleController();

	void HandleSetupTitleHUD();

	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnDeleteGame();

	UFUNCTION()
	void OnBackToMainMenu();

	// FadeInOut
	UFUNCTION(BlueprintCallable)
	void FadeIn();

	UFUNCTION(BlueprintCallable)
	void FadeOut();

	FORCEINLINE float GetFadeDuration() const { return FadeDuration; }
	
protected:
	virtual void BeginPlay() override;

	void CreateHUDWidget();
	void AddHUDToViewport() const;
	void SetupInputModeGameAndUI();
	void SetupMappingContext() const;

	void BindControllerInputActions();
	

protected:
	float FadeDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UNewTitleMenuWidget> TitleHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UNewTitleMenuWidget> TitleHUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UFadeWidget> FadeWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UFadeWidget> FadeWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> TitleMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> StartGame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> DeleteGame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> BackToMainMenu;
	
	UPROPERTY()
	TObjectPtr<class UEnhancedInputLocalPlayerSubsystem> LocalPlayerInputSubsystem;
	
};
