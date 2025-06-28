// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UI/UITitle/NewTitleMenuWidget.h"
#include "UI/FadeInOut/FadeWidget.h"
#include "Util/DevCheatManager.h"

ATitleController::ATitleController()
{
	static ConstructorHelpers::FClassFinder<UNewTitleMenuWidget> TitleMenuWidgetRef (TEXT("/Game/Blueprints/UI/NewUI/StartMenuWidget.StartMenuWidget_C"));
	if (TitleMenuWidgetRef.Class)
	{
		TitleHUDWidgetClass = TitleMenuWidgetRef.Class;
	}

	CheatClass = UDevCheatManager::StaticClass();
	FadeDuration = 1.f;
}

void ATitleController::HandleSetupTitleHUD()
{
	SetupMappingContext();
	CreateHUDWidget();
	AddHUDToViewport();
	SetupInputModeGameAndUI();

	BindControllerInputActions();

	FadeIn();
}

void ATitleController::OnStartGame()
{
	OnPressedStartGame.ExecuteIfBound();
	UE_LOG(LogTemp, Warning, TEXT("OnStartGame"));
}

void ATitleController::OnDeleteGame()
{
	OnPressedDeleteGame.ExecuteIfBound();
	UE_LOG(LogTemp, Warning, TEXT("OnDeleteGame"));
}

void ATitleController::OnBackToMainMenu()
{
	OnPressedBackToMainMenu.ExecuteIfBound();
	UE_LOG(LogTemp, Warning, TEXT("OnBackToMainMenu"));
}

void ATitleController::FadeIn()
{
	check(FadeWidgetInstance);
	FadeWidgetInstance->FadeIn(FadeDuration);
}

void ATitleController::FadeOut()
{
	check(FadeWidgetInstance);
	FadeWidgetInstance->FadeOut(FadeDuration);
}

void ATitleController::SetupInputModeGameAndUI()
{
	const FInputModeGameAndUI CurrentInputMode;
	SetInputMode(CurrentInputMode);
	bShowMouseCursor = true;

	// Focus Setting -> HUD Widget, not Controller
}

void ATitleController::SetupMappingContext() const
{
	if (LocalPlayerInputSubsystem)
	{
		if (TitleMappingContext)
		{
			LocalPlayerInputSubsystem->AddMappingContext(TitleMappingContext, 0);
		}
	}
}

void ATitleController::BindControllerInputActions()
{
	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComponent)
	{
		if (IsValid(StartGame))
		{
			EnhancedInputComponent->BindAction(StartGame, ETriggerEvent::Started, this, &ATitleController::OnStartGame);
		}

		if (IsValid(DeleteGame))
		{
			EnhancedInputComponent->BindAction(DeleteGame, ETriggerEvent::Started, this, &ATitleController::OnDeleteGame);
		}

		if (IsValid(BackToMainMenu))
		{
			EnhancedInputComponent->BindAction(BackToMainMenu, ETriggerEvent::Started, this, &ATitleController::OnBackToMainMenu);
		}
	}
	
}

void ATitleController::BeginPlay()
{
	Super::BeginPlay();

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer)
	{
		LocalPlayerInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	}

	HandleSetupTitleHUD();
}

void ATitleController::CreateHUDWidget()
{
	check(TitleHUDWidgetClass);
	check(FadeWidgetClass);
	
	TitleHUDWidgetInstance = CreateWidget<UNewTitleMenuWidget>(this, TitleHUDWidgetClass);
	FadeWidgetInstance = CreateWidget<UFadeWidget>(this, FadeWidgetClass);
}

void ATitleController::AddHUDToViewport() const
{
	check(TitleHUDWidgetInstance);
	check(FadeWidgetInstance);
	
	TitleHUDWidgetInstance->AddToViewport();
	FadeWidgetInstance->AddToViewport();
}
