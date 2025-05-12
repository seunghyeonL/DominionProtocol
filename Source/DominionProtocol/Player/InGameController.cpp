// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameController.h"
#include "Util/DevCheatManager.h"
#include "UI/DomiInGameHUDWidget.h"

AInGameController::AInGameController()
{
	static ConstructorHelpers::FClassFinder<UDomiInGameHUDWidget> InGameHUDWidgetRef (TEXT("/Game/Blueprints/UI/WBP_DomiInGameHUDWidget.WBP_DomiInGameHUDWidget_C"));
	if (InGameHUDWidgetRef.Class)
	{
		InGameHUDWidgetClass = InGameHUDWidgetRef.Class;	
	}
	
	CheatClass = UDevCheatManager::StaticClass();
}

void AInGameController::HandleSetupInGameHUD()
{
	CreateHUDWidget();
	AddHUDToViewport();
	SetupInputModeGameAndUI();
}

void AInGameController::BeginPlay()
{
	Super::BeginPlay();

	HandleSetupInGameHUD();
}

void AInGameController::CreateHUDWidget()
{
	check(InGameHUDWidgetClass);

	InGameHUDWidgetInstance = CreateWidget<UDomiInGameHUDWidget>(this, InGameHUDWidgetClass);
}

void AInGameController::AddHUDToViewport() const
{
	check(InGameHUDWidgetInstance);

	InGameHUDWidgetInstance->AddToViewport();
}

void AInGameController::SetupInputModeGameAndUI()
{
	const FInputModeGameAndUI CurrentInputMode;
	SetInputMode(CurrentInputMode);
	bShowMouseCursor = false;

	// Focus Setting -> HUD Widget, not Controller
}
