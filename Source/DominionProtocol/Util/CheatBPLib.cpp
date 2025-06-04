// Fill out your copyright notice in the Description page of Project Settings.


#include "CheatBPLib.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "DomiFramework/GameInstance/SaveManagerSubsystem.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/ExponentialHeightFog.h"

#include "Util/DebugHelper.h"
#include "Kismet/GameplayStatics.h"

void UCheatBPLib::Save(const UWorld* World)
{
	USaveManagerSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (SaveSubsystem->SaveGame("SaveGame1", 0))
	{
		Debug::Print("Save Game Success");
	}

	UWorldInstanceSubsystem* WorldInstanceSubsystem = World->GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
	Debug::Print(TEXT("Save Game in SaveGame1"));
	Debug::Print(FString::Printf(TEXT("SaveData[CurrentLevelName] : %s"), *WorldInstanceSubsystem->GetCurrentLevelName()));
}

void UCheatBPLib::InfiniteStamina(const TObjectPtr<UStatusComponent> StatusComponent)
{
	if (IsValid(StatusComponent))
	{
		StatusComponent->SwitchInfiniteStaminaMode();
	}
}

void UCheatBPLib::MoveToCrack(ABaseGameMode* BaseGameMode, FString TargetLevelName, int32 TargetCrackIndex)
{
	BaseGameMode->MoveToTargetCrack(TargetLevelName, TargetCrackIndex);
}

void UCheatBPLib::AddAllItemsToPlayerInventoryMaxQuantity(UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!IsValid(PlayerController))
	{
		return;
	}

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(PlayerController->GetPawn());
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	UItemComponent* ItemComponent = PlayerCharacter->FindComponentByClass<UItemComponent>();
	if (IsValid(ItemComponent))
	{
		ItemComponent->AddAllItemsToInventoryMaxQuantity();
	}
	else
	{
	}
}

void UCheatBPLib::ToggleFog(UWorld* World)
{
	AExponentialHeightFog* Fog = Cast<AExponentialHeightFog>(UGameplayStatics::GetActorOfClass(World, AExponentialHeightFog::StaticClass()));
	if (IsValid(Fog))
	{
		UExponentialHeightFogComponent* FogComp = Fog->GetComponent();
		if (FogComp)
		{
			float CurrentOpacity = FogComp->FogMaxOpacity;
			float NewOpacity = (CurrentOpacity > 0.f) ? 0.f : 1.f;
			FogComp->FogMaxOpacity = NewOpacity;
			FogComp->SetFogMaxOpacity(NewOpacity);
			FogComp->MarkRenderStateDirty();
			
			Debug::Print(FString::Printf(TEXT("Fog Opacity toggled, New Opacity : %f"), NewOpacity));
		}
	}
}
