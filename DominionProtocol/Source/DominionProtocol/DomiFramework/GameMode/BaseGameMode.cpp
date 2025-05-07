// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"
#include "Kismet/GameplayStatics.h"

ABaseGameMode::ABaseGameMode()
{/*(
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
	GameInstance = GetGameInstance();

	APawn* Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (Pawn && !Pawn->ActorHasTag("Player"))
	{
		Pawn->Tags.Add("Player");
	}
}
