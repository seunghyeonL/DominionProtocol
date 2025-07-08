// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleController.h"

#include "EnhancedInputSubsystems.h"
#include "UI/UITitle/NewTitleHUDWidget.h"
#include "UI/FadeInOut/FadeWidget.h"
#include "Util/DevCheatManager.h"

ATitleController::ATitleController()
{
	static ConstructorHelpers::FClassFinder<UNewTitleHUDWidget> TitleMenuWidgetRef (TEXT("/Game/Blueprints/UI/NewUI/StartMenuWidget.StartMenuWidget_C"));
	if (TitleMenuWidgetRef.Class)
	{
		TitleHUDWidgetClass = TitleMenuWidgetRef.Class;
	}

	CheatClass = UDevCheatManager::StaticClass();
}

void ATitleController::OnStartGame()
{
	OnPressedStartGame.ExecuteIfBound();
}

void ATitleController::OnDeleteGame()
{
	OnPressedDeleteGame.ExecuteIfBound();
}

void ATitleController::OnBackToMainMenu()
{
	OnPressedBackToMainMenu.ExecuteIfBound();
}

void ATitleController::BeginPlay()
{
	Super::BeginPlay();

	FadeIn();
}

void ATitleController::CreateAndAddHUDWidget()
{
	Super::CreateAndAddHUDWidget();

	check(TitleHUDWidgetClass);
	
	TitleHUDWidgetInstance = CreateWidget<UNewTitleHUDWidget>(this, TitleHUDWidgetClass);

	if (TitleHUDWidgetInstance)
	{
		TitleHUDWidgetInstance->AddToViewport();	
	}
}

void ATitleController::SetupInputMode()
{
	Super::SetupInputMode();

	const FInputModeGameAndUI CurrentInputMode;
	SetInputMode(CurrentInputMode);
	bShowMouseCursor = true;
}

void ATitleController::SetupMappingContext()
{
	Super::SetupMappingContext();

	if (LocalPlayerInputSubsystem)
	{
		if (TitleMappingContext)
		{
			LocalPlayerInputSubsystem->AddMappingContext(TitleMappingContext, 0);
		}
	}
}

void ATitleController::BindInputActions()
{
	Super::BindInputActions();

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	HelperBindInputAction(EnhancedInputComponent, StartGame, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnStartGame));
	HelperBindInputAction(EnhancedInputComponent, DeleteGame, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnDeleteGame));
	HelperBindInputAction(EnhancedInputComponent, BackToMainMenu, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnBackToMainMenu));
}
