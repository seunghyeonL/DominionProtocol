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

void AInGameController::BeginPlay()
{
	Super::BeginPlay();

	CreateHUDWidget();
	AddHUDToViewport();
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
