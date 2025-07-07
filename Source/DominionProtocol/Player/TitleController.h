// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePlayerController.h"
#include "TitleController.generated.h"

DECLARE_DELEGATE(FOnPressedStartGame)
DECLARE_DELEGATE(FOnPressedDeleteGame)
DECLARE_DELEGATE(FOnPressedBackToMainMenu)

UCLASS()
class DOMINIONPROTOCOL_API ATitleController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	FOnPressedStartGame OnPressedStartGame;
	FOnPressedDeleteGame OnPressedDeleteGame;
	FOnPressedBackToMainMenu OnPressedBackToMainMenu;
	
	ATitleController();

	UFUNCTION()
	void OnStartGame();

	UFUNCTION()
	void OnDeleteGame();

	UFUNCTION()
	void OnBackToMainMenu();

	FORCEINLINE float GetFadeDuration() const { return FadeDuration; }
	
protected:
	virtual void BeginPlay() override;

	virtual void CreateAndAddHUDWidget() override;
	virtual void SetupInputMode() override;
	virtual void SetupMappingContext() override;
	virtual void BindInputActions() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UNewTitleMenuWidget> TitleHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UNewTitleMenuWidget> TitleHUDWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputMappingContext> TitleMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> StartGame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> DeleteGame;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<class UInputAction> BackToMainMenu;
	
};
