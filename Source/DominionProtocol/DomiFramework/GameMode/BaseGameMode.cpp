// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Util/DebugHelper.h"

ABaseGameMode::ABaseGameMode()
{/*
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
}

void ABaseGameMode::StartPlay()
{
	Super::StartPlay();
	GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	check(GameInstance);

	if (!GameInstance)
	{
		Debug::Print(TEXT("GI Fail"));
	}

	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (IsValid(Pawn) && !Pawn->ActorHasTag("Player"))
	{
		Pawn->Tags.Add("Player");
	}
}

void ABaseGameMode::StartBattle()
{
}

void ABaseGameMode::EndBattle()
{
}
