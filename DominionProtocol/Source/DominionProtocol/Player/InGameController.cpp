// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameController.h"
#include "Util/DevCheatManager.h"

AInGameController::AInGameController()
{
	CheatClass = UDevCheatManager::StaticClass();
}
