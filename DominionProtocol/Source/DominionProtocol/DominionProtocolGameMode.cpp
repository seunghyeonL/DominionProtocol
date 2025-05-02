// Copyright Epic Games, Inc. All Rights Reserved.

#include "DominionProtocolGameMode.h"
#include "DominionProtocolCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADominionProtocolGameMode::ADominionProtocolGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
