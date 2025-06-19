// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleController.h"

#include "Blueprint/UserWidget.h"
#include "Util/DevCheatManager.h"

ATitleController::ATitleController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> StartMenuWidgetRef (TEXT("/Game/Blueprints/UI/NewUI/StartMenuWidget.StartMenuWidget_C"));
	if (StartMenuWidgetRef.Class)
	{
		TitleHUDWidgetClass = StartMenuWidgetRef.Class;
	}

	CheatClass = UDevCheatManager::StaticClass();
}

void ATitleController::HandleSetupTitleHUD()
{
	CreateHUDWidget();
	AddHUDToViewport();
	SetupInputModeUIOnly();
}

void ATitleController::SetupInputModeUIOnly()
{
	const FInputModeUIOnly CurrentInputMode;
	SetInputMode(CurrentInputMode);
	bShowMouseCursor = true;

	// Focus Setting -> HUD Widget, not Controller
}

void ATitleController::BeginPlay()
{
	Super::BeginPlay();

	HandleSetupTitleHUD();
}

void ATitleController::CreateHUDWidget()
{
	check(TitleHUDWidgetClass);
	
	TitleHUDWidgetInstance = CreateWidget<UUserWidget>(this, TitleHUDWidgetClass);
}

void ATitleController::AddHUDToViewport() const
{
	check(TitleHUDWidgetInstance)
	
	TitleHUDWidgetInstance->AddToViewport();
}
