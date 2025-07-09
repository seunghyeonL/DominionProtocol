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

	MappingContextArray.AddUnique(TitleMenuUIMappingContext);
	MappingContextArray.AddUnique(TitleSlotUIMappingContext);

	CheatClass = UDevCheatManager::StaticClass();
}

void ATitleController::OnTitleSlotUIStartGame() const
{
	OnStartGame.Broadcast();
}

void ATitleController::OnTitleSlotUIDeleteGame() const
{
	OnDeleteGame.Broadcast();
}

void ATitleController::OnTitleSlotUIBackToTitleMenu() const
{
	OnBackToTitleMenu.Broadcast();
}

void ATitleController::OnTitleMenuUIMoveSelectionUp() const
{
	OnMoveSelectionUp.Broadcast();
}

void ATitleController::OnTitleMenuUIMoveSelectionDown() const
{
	OnMoveSelectionDown.Broadcast();
}

void ATitleController::OnTitleMenuUIConfirmSelection() const
{
	OnConfirmSelection.Broadcast();
}

void ATitleController::SetupMappingContext(const UInputMappingContext* NewInputMappingContext)
{
	if (LocalPlayerInputSubsystem)
	{
		RemoveAllMappingContext();
		if (!LocalPlayerInputSubsystem->HasMappingContext(NewInputMappingContext))
		{
			LocalPlayerInputSubsystem->AddMappingContext(NewInputMappingContext,1);
		}
	}
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

}

void ATitleController::BindInputActions()
{
	Super::BindInputActions();

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	check(EnhancedInputComponent);

	HelperBindInputAction(EnhancedInputComponent, InputTitleSlotUIStartGame, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnTitleSlotUIStartGame));
	HelperBindInputAction(EnhancedInputComponent, InputTitleSlotUIDeleteGame, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnTitleSlotUIDeleteGame));
	HelperBindInputAction(EnhancedInputComponent, InputTitleSlotUIBackToTitleMenu, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnTitleSlotUIBackToTitleMenu));

	HelperBindInputAction(EnhancedInputComponent, InputTitleMenuUIMoveSelectionUp, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnTitleMenuUIMoveSelectionUp));
	HelperBindInputAction(EnhancedInputComponent, InputTitleMenuUIMoveSelectionDown, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnTitleMenuUIMoveSelectionDown));
	HelperBindInputAction(EnhancedInputComponent, InputTitleMenuUIConfirmSelection, ETriggerEvent::Started, GET_FUNCTION_NAME_CHECKED(ATitleController, OnTitleMenuUIConfirmSelection));
}
