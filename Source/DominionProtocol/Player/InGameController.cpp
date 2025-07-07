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

void AInGameController::OnMainMenuSwitchShowAndHideWidget()
{
	OnPressedMainMenuSwitchShowAndHideWidgetEvent.ExecuteIfBound();
}

void AInGameController::OnDialogueChangedNextStoryState()
{
	OnPressedDialogueChangedNextStoryState.ExecuteIfBound();
}

void AInGameController::OnPressedCrackMenuBackButton()
{
	OnPressedCrackMenuBackButtonEvent.ExecuteIfBound();
}

void AInGameController::OnPressedCrackMenuConfirmButton()
{
	OnPressedCrackMenuConfirmButtonEvent.ExecuteIfBound();
}

void AInGameController::OnPressedMainMenuButtonQ()
{
	OnPressedMainMenuButtonQEvent.ExecuteIfBound();
}

void AInGameController::OnPressedMainMenuButtonE()
{
	OnPressedMainMenuButtonEEvent.ExecuteIfBound();
}

void AInGameController::OnPressedMainMenuButtonR()
{
	OnPressedMainMenuButtonREvent.ExecuteIfBound();
}

void AInGameController::OnPressedMainMenuButtonA()
{
	OnPressedMainMenuButtonAEvent.ExecuteIfBound();
}

void AInGameController::OnPressedMainMenuButtonD()
{
	OnPressedMainMenuButtonDEvent.ExecuteIfBound();
}

void AInGameController::OnPressedMainMenuButtonZ()
{
	OnPressedMainMenuButtonZEvent.ExecuteIfBound();
}

void AInGameController::OnPressedMainMenuButtonC()
{
	OnPressedMainMenuButtonCEvent.ExecuteIfBound();
}

void AInGameController::OnPressedMainMenuButtonSpaceBar()
{
	OnPressedMainMenuButtonSpaceBarEvent.ExecuteIfBound();
}

void AInGameController::RemoveAllMappingContext()
{
	for (const auto* MappingContext : MappingContextArray)
	{
		LocalPlayerInputSubsystem->RemoveMappingContext(MappingContext);
	}
}

void AInGameController::BeginPlay()
{
	Super::BeginPlay();
	
	MappingContextArray.AddUnique(DefaultMappingContext);
	MappingContextArray.AddUnique(MainMenuMappingContext);
	MappingContextArray.AddUnique(DialogueMappingContext);
	MappingContextArray.AddUnique(CrackMenuMappingContext);

	FadeIn();
}

void AInGameController::CreateAndAddHUDWidget()
{
	Super::CreateAndAddHUDWidget();
	
	InGameHUDWidgetInstance = CreateWidget<UDomiInGameHUDWidget>(this, InGameHUDWidgetClass);

	if (InGameHUDWidgetInstance)
	{
		InGameHUDWidgetInstance->AddToViewport();
	}
}

void AInGameController::SetupInputMode()
{
	Super::SetupInputMode();

	const FInputModeGameAndUI CurrentInputMode;
	SetInputMode(CurrentInputMode);
	bShowMouseCursor = false;
}

void AInGameController::SetupMappingContext()
{
	Super::SetupMappingContext();

	if (LocalPlayerInputSubsystem)
	{
		if (DefaultMappingContext)
		{
			LocalPlayerInputSubsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AInGameController::BindInputActions()
{
	Super::BindInputActions();

	auto* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComp);

	// Main Menu UI Input
	HelperBindInputAction(EnhancedInputComp, MainMenuSwitchShowAndHideWidget, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnMainMenuSwitchShowAndHideWidget));
	HelperBindInputAction(EnhancedInputComp, MainMenuPressButtonQ, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedMainMenuButtonQ));
	HelperBindInputAction(EnhancedInputComp, MainMenuPressButtonE, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedMainMenuButtonE));
	HelperBindInputAction(EnhancedInputComp, MainMenuPressButtonR, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedMainMenuButtonR));
	HelperBindInputAction(EnhancedInputComp, MainMenuPressButtonA, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedMainMenuButtonA));
	HelperBindInputAction(EnhancedInputComp, MainMenuPressButtonD, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedMainMenuButtonD));
	HelperBindInputAction(EnhancedInputComp, MainMenuPressButtonZ, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedMainMenuButtonZ));
	HelperBindInputAction(EnhancedInputComp, MainMenuPressButtonC, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedMainMenuButtonC));
	HelperBindInputAction(EnhancedInputComp, MainMenuPressButtonSpaceBar, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedMainMenuButtonSpaceBar));

	// Dialogue UI Input
	HelperBindInputAction(EnhancedInputComp, DialogueChangeNextStoryState, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnDialogueChangedNextStoryState));
	
	// Crack UI Input
	HelperBindInputAction(EnhancedInputComp, PressedCrackBackButton, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedCrackMenuBackButton));
	HelperBindInputAction(EnhancedInputComp, PressedCrackConfirmButton, ETriggerEvent::Triggered, GET_FUNCTION_NAME_CHECKED(AInGameController, OnPressedCrackMenuConfirmButton));
	
}

void AInGameController::SetupMappingContext(class UInputMappingContext* NewMappingContext)
{
	if (LocalPlayerInputSubsystem)
	{
		// Mapping Context Add & Remove
		RemoveAllMappingContext();
		if (!LocalPlayerInputSubsystem->HasMappingContext(NewMappingContext))
		{
			LocalPlayerInputSubsystem->AddMappingContext(NewMappingContext, 1);	
		}
		
		// Setting InputMode 
		if (DefaultMappingContext == NewMappingContext)
		{
			const FInputModeGameOnly CurrentInputMode;
			SetInputMode(CurrentInputMode);
			bShowMouseCursor = false;
		}
		else
		{
			const FInputModeGameAndUI CurrentInputMode;
			SetInputMode(CurrentInputMode);
			bShowMouseCursor = true;
		}

	}
}
