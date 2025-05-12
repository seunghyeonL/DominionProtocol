// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleController.h"
#include "UI/UITitle/DomiTitleHUDWidget.h"

ATitleController::ATitleController()
{
	static ConstructorHelpers::FClassFinder<UDomiTitleHUDWidget> TitleHUDWidgetRef (TEXT("/Game/Blueprints/UI/WBP_DomiTitleHUDWidget.WBP_DomiTitleHUDWidget_C"));
	if (TitleHUDWidgetRef.Class)
	{
		TitleHUDWidgetClass = TitleHUDWidgetRef.Class;
	}
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
	
	TitleHUDWidgetInstance = CreateWidget<UDomiTitleHUDWidget>(this, TitleHUDWidgetClass);
}

void ATitleController::AddHUDToViewport() const
{
	check(TitleHUDWidgetInstance)
	
	TitleHUDWidgetInstance->AddToViewport();
}
