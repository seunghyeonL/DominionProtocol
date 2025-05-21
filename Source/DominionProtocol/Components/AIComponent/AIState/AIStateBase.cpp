// Fill out your copyright notice in the Description page of Project Settings.


#include "AIStateBase.h"
#include "Components/AIComponent/AIStateComponent.h"

void UAIStateBase::Activate()
{
	bIsActive = true;
	UE_LOG(LogTemp, Warning, TEXT("AI State Activated: %s"), *StateTag.ToString());
}

void UAIStateBase::Deactivate()
{
	bIsActive = false;
	UE_LOG(LogTemp, Warning, TEXT("AI State Deactivated: %s"), *StateTag.ToString());
}
