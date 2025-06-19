// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitleController.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API ATitleController : public APlayerController
{
	GENERATED_BODY()

public:
	ATitleController();

	void HandleSetupTitleHUD();
	
protected:
	virtual void BeginPlay() override;

	void CreateHUDWidget();
	void AddHUDToViewport() const;
	void SetupInputModeUIOnly();
	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> TitleHUDWidgetClass;

	UPROPERTY()
	TObjectPtr<class UUserWidget> TitleHUDWidgetInstance;
};
