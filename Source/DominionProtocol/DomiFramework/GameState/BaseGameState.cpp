// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "GameFramework/Character.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "Components/StatusComponent/StatusComponentInitializeData.h"
#include "EnumAndStruct/FCrackData.h"
#include "WorldObjects/Crack.h"
#include "Kismet/GameplayStatics.h"

#include "Util/DebugHelper.h"

ABaseGameState::ABaseGameState()
	:	GameInstance(nullptr),
		SoundSubsystem(nullptr),
		SkillDataTable(nullptr),
		SkillInitializeDataTable(nullptr),
		StatusInitializeDataTable(nullptr)
{
}

void ABaseGameState::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
	check(World);
	InitializeGameInstance();
	InitializeSoundSubsystem();
}

void ABaseGameState::InitializeGameInstance()
{
	GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	check(GameInstance);
	GameInstance->SetCurrentLevelName(UGameplayStatics::GetCurrentLevelName(GetWorld(), true));
	if (GameInstance->GetCurrentLevelName().Contains("Level1"))
	{
		GameInstance->SetCurrentLevelDisplayName(FText::FromString("Level 1"));
	}
	else if (GameInstance->GetCurrentLevelName().Contains("Level 2"))
	{
		GameInstance->SetCurrentLevelDisplayName(FText::FromString("Level 2"));
	}
}

void ABaseGameState::InitializeSoundSubsystem()
{
	SoundSubsystem = GetGameInstance()->GetSubsystem<USoundInstanceSubsystem>();
	check(SoundSubsystem);
	SoundSubsystem->SetWorldCache(GetWorld());
	SoundSubsystem->LoadSoundClass();
	SoundSubsystem->LoadVolumeSettings();
}

FSkillData* ABaseGameState::GetSkillData(const FGameplayTag SkillTag) const
{
	check(SkillDataTable);
	
	return SkillDataTable->FindRow<FSkillData>(SkillTag.GetTagName(), TEXT(""));
}

FSkillComponentInitializeData* ABaseGameState::GetSkillComponentInitializeData(const FGameplayTag PawnTag) const
{
	check(SkillInitializeDataTable);
	
	return SkillInitializeDataTable->FindRow<FSkillComponentInitializeData>(PawnTag.GetTagName(), TEXT(""));
}

FStatusComponentInitializeData* ABaseGameState::GetStatusComponentInitializeData(const FGameplayTag PawnTag) const
{
	check(StatusInitializeDataTable);
	
	return StatusInitializeDataTable->FindRow<FStatusComponentInitializeData>(PawnTag.GetTagName(), TEXT(""));
}

void ABaseGameState::CacheAllCracks()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ACrack::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ACrack* Crack = Cast<ACrack>(Actor);
		if (IsValid(Crack))
		{
			AllCracksCache.Add(Crack);
		}
	}

	AllCracksCache.Sort([](const ACrack& A, const ACrack& B)
	{
		return A.GetCrackIndex() < B.GetCrackIndex();
	});
}

void ABaseGameState::InitializeCrackDataMap()
{
	TMap<FString, FCrackDataArray> NewCrackDataMap;
	FCrackDataArray NewCrackDataArray;

	for (ACrack* Crack : AllCracksCache)
	{
		FCrackData NewCrackData;

		if (Crack->GetCrackIndex() == 0)
		{
			Crack->SetActive();
		}
		
		NewCrackData.CrackName = Crack->GetCrackName();
		NewCrackData.bIsActivate = Crack->GetIsActivate();
		NewCrackData.RespawnLocation = Crack->GetRespawnTargetPointLocation();
		NewCrackData.RespawnRotation = Crack->GetRespawnTargetPointRotation();

		NewCrackDataArray.CrackDataArray.Add(NewCrackData);
	}
	NewCrackDataMap.Add(GameInstance->GetCurrentLevelName(), NewCrackDataArray);

	GameInstance->SetCrackDataMap(NewCrackDataMap);
	AddAnotherLevelFirstCrack();
}

void ABaseGameState::AddAnotherLevelFirstCrack()
{
	TMap<FString, FCrackDataArray>* NewCrackDataMap = GameInstance->GetCrackDataMap();
	FCrackDataArray NewCrackDataArray;
	FCrackData NewCrackData;

	if (GameInstance->GetCurrentLevelName().Contains("Level1"))
	{
		NewCrackData.CrackName = FText::FromString("Level2 Crack0");
		NewCrackData.bIsActivate = true;
		NewCrackData.RespawnLocation = FVector(240, 0, 20);
		NewCrackData.RespawnRotation = FRotator(0, 0, 0);
		NewCrackDataArray.CrackDataArray.Add(NewCrackData);
		NewCrackDataMap->Add(FString("Proto_Level2"), NewCrackDataArray);
		NewCrackDataMap->Add(FString("TestCrackLevel2"), NewCrackDataArray);
	}
	else if (GameInstance->GetCurrentLevelName().Contains("Level2"))
	{
		NewCrackData.CrackName = FText::FromString("Level1 Crack0");
		NewCrackData.bIsActivate = true;
		NewCrackData.RespawnLocation = FVector(240, 0, 20);
		NewCrackData.RespawnRotation = FRotator(0, 0, 0);
		NewCrackDataArray.CrackDataArray.Add(NewCrackData);
		NewCrackDataMap->Add(FString("Proto_Level1"), NewCrackDataArray);
		NewCrackDataMap->Add(FString("TestCrackLevel1"), NewCrackDataArray);
	}
	
	GameInstance->SetCrackDataMap(*NewCrackDataMap);
}

ACrack* ABaseGameState::FindNearestCrack()
{
	ACharacter* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetCharacter();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	ACrack* NearestCrack = nullptr;
	float NearestDistanceSquared = TNumericLimits<float>::Max();
	
	for (ACrack* Crack : AllCracksCache)
	{
		float DistanceSquared = FVector::DistSquared(PlayerLocation, Crack->GetActorLocation());
		if (DistanceSquared < NearestDistanceSquared)
		{
			NearestCrack = Crack;
			NearestDistanceSquared = DistanceSquared;
		}
	}

	return NearestCrack;
}
