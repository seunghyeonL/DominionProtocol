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
	
	MappingContextArray.AddUnique(DefaultMappingContext);
	MappingContextArray.AddUnique(MainMenuMappingContext);
	MappingContextArray.AddUnique(DialogueMappingContext);
	MappingContextArray.AddUnique(CrackMenuMappingContext);
	
	SetupMappingContext(DefaultMappingContext);

	BindControllerInputActions();
}

void AInGameController::OnMainMenuSwitchShowAndHideWidget()
{
	InGameHUDWidgetInstance->OnMainMenuSwitchShowAndHideWidget();
}

void AInGameController::OnDialogueChangedNextStoryState()
{
	InGameHUDWidgetInstance->OnDialogueChangedNextStoryState();
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


void AInGameController::BindControllerInputActions()
{
	auto* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent);
	if (EnhancedInputComp)
	{
		if (IsValid(MainMenuSwitchShowAndHideWidget))
		{
			EnhancedInputComp->BindAction(MainMenuSwitchShowAndHideWidget, ETriggerEvent::Started,
				this,
				&AInGameController::OnMainMenuSwitchShowAndHideWidget);
		}

		if (IsValid(DialogueChangeNextStoryState))
		{
			EnhancedInputComp->BindAction(DialogueChangeNextStoryState, ETriggerEvent::Started,
				this,
				&AInGameController::OnDialogueChangedNextStoryState);
		}
	}
}
