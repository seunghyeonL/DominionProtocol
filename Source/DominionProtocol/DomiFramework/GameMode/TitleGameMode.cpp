// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiFramework/GameMode/TitleGameMode.h"

ATitleGameMode::ATitleGameMode()
{
	static ConstructorHelpers::FClassFinder<APlayerController> TitleControllerClassRef(TEXT("/Script/DominionProtocol.TitleController"));
	if (TitleControllerClassRef.Class)
	{
		PlayerControllerClass = TitleControllerClassRef.Class;	
	}
}
