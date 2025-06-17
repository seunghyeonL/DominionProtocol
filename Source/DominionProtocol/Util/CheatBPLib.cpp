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

void UCheatBPLib::Save(const UWorld* World, int32 SlotIndex)
{
	USaveManagerSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (IsValid(SaveSubsystem))
	{
		bool bSaveSuccess = SaveSubsystem->SaveGame(FString::Printf(TEXT("SaveGame%d"), SlotIndex + 1), SlotIndex);
		if (bSaveSuccess)
		{
			Debug::Print("Save Game Success");
		}
	}
}

void UCheatBPLib::Load(const UWorld* World, int32 SlotIndex)
{
	USaveManagerSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (IsValid(SaveSubsystem))
	{
		bool bLoadSuccess = SaveSubsystem->LoadGame(FString::Printf(TEXT("SaveGame%d"), SlotIndex + 1), SlotIndex);
		if (bLoadSuccess)
		{
			Debug::Print(TEXT("Load Game Success"));
		}
	}
}

void UCheatBPLib::StartGameNewSlot(const UWorld* World, int32 SlotIndex)
{
	USaveManagerSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (IsValid(SaveSubsystem))
	{
		SaveSubsystem->StartNewGame(SlotIndex);
	}
}

void UCheatBPLib::SaveSlotLoadAndStartGame(const UWorld* World, int32 SlotIndex)
{
	USaveManagerSubsystem* SaveSubsystem = World->GetGameInstance()->GetSubsystem<USaveManagerSubsystem>();
	if (IsValid(SaveSubsystem))
	{
		SaveSubsystem->LoadSaveDataAndOpenLevel(SlotIndex);
	}
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

void UCheatBPLib::ForceMoveToCrack(UWorld* World, int32 TargetCrackIndex, ADomiCharacter* Character)
{
	UWorldInstanceSubsystem* WorldInstanceSubsystem =  World->GetGameInstance()->GetSubsystem<UWorldInstanceSubsystem>();
	const FCrackData* TargetCrackData = WorldInstanceSubsystem->GetCrackData(WorldInstanceSubsystem->GetCurrentLevelName(), TargetCrackIndex);
	if (TargetCrackData)
	{
		Character->SetActorLocation(TargetCrackData->RespawnLocation);
		Character->SetActorRotation(TargetCrackData->RespawnRotation);
	}
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

void UCheatBPLib::EquipSword(UObject* WorldContextObject)
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
		ItemComponent->AddItem(ItemTags::SwordWeapon,1);
		ItemComponent->EquipItem(FName("WeaponSlot_Primary"),ItemTags::SwordWeapon);
	}
}

void UCheatBPLib::EquipAxe(UObject* WorldContextObject)
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
		ItemComponent->AddItem(ItemTags::AxeWeapon,1);
		ItemComponent->EquipItem(FName("WeaponSlot_Primary"),ItemTags::AxeWeapon);
	}
}

void UCheatBPLib::ToggleFog(UObject* WorldContextObject)
{
	if (!WorldContextObject) return;

	UWorld* World = WorldContextObject->GetWorld();
	
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
