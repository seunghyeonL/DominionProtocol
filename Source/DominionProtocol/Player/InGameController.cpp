// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Util/DevCheatManager.h"
#include "UI/UIInGame/DomiInGameHUDWidget.h"

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
	SetupInputModeGameOnly();

	BindControllerInputActions();
}

void AInGameController::OnInGameMenuOpenAndClose()
{
	if (bActiveInGameMenuOpen)
	{
		SetupInputModeGameOnly();
	}
	else
	{
		SetupInputModeUIOnly();
	}

	bActiveInGameMenuOpen = !bActiveInGameMenuOpen;
	InGameHUDWidgetInstance->OnInGameMenuOpenAndClose();
}

void AInGameController::BeginPlay()
{
	Super::BeginPlay();

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer)
	{
		LocalPlayerInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	}

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

void AInGameController::SetupInputModeGameOnly()
{
	if (LocalPlayerInputSubsystem)
	{
		LocalPlayerInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
		LocalPlayerInputSubsystem->RemoveMappingContext(UIOnlyMappingContext);

		
		const FInputModeGameOnly CurrentInputMode;
		SetInputMode(CurrentInputMode);
		bShowMouseCursor = false;

	}
}

void AInGameController::SetupInputModeUIOnly()
{
	if (LocalPlayerInputSubsystem)
	{
		LocalPlayerInputSubsystem->AddMappingContext(UIOnlyMappingContext, 1000);
		LocalPlayerInputSubsystem->RemoveMappingContext(DefaultMappingContext);

		const FInputModeGameAndUI CurrentInputMode;
		SetInputMode(CurrentInputMode);
		bShowMouseCursor = true;
		
	}
}

void AInGameController::BindControllerInputActions()
{
	auto* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComp)
	{
		if (IsValid(InGameMenuOpenAndClose))
		{
			EnhancedInputComp->BindAction(InGameMenuOpenAndClose, ETriggerEvent::Started,
				this,
				&AInGameController::OnInGameMenuOpenAndClose);
		}
	}
}
