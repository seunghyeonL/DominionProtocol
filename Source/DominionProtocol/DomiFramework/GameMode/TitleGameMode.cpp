// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiFramework/GameMode/TitleGameMode.h"
#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"

#include "Util/DevCheatManager.h"

ATitleGameMode::ATitleGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> TitleControllerClassRef(TEXT("/Script/DominionProtocol.TitleController"));
	if (TitleControllerClassRef.Class)
	{
		PlayerControllerClass = TitleControllerClassRef.Class;
	}
}

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	USaveManagerSubsystem* SaveManagerSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	check(IsValid(SaveManagerSubsystem));

	SaveManagerSubsystem->SetWorldCache(GetWorld());
	SaveManagerSubsystem->LoadSettings();
}
