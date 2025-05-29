// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "GameFramework/Character.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"
#include "DomiFramework/WorldActorLoader/WorldActorLoaderSubsystem.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "Components/StatusComponent/StatusComponentInitializeData.h"
#include "EnumAndStruct/FCrackData.h"
#include "EnumAndStruct/ECategory.h"
#include "EnumAndStruct/FWorldActorData.h"
#include "WorldObjects/Crack.h"
#include "Kismet/GameplayStatics.h"
#include "WorldObjects/LightWeightActor.h"

#include "Util/DebugHelper.h"


ABaseGameState::ABaseGameState()
	: GameInstance(nullptr),
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
	InitializeWorldInstanceSubsystem();
	InitializeZeroIndexCrackData(WorldInstanceSubsystem->GetCurrentLevelName());
	InitializeSoundSubsystem();
}

void ABaseGameState::InitializeGameInstance()
{
	GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	check(GameInstance);
}

void ABaseGameState::InitializeWorldInstanceSubsystem()
{
	WorldInstanceSubsystem = GameInstance->GetSubsystem<UWorldInstanceSubsystem>();
	check(WorldInstanceSubsystem);

	WorldInstanceSubsystem->SetCurrentLevelName(UGameplayStatics::GetCurrentLevelName(GetWorld(), true));
	if (WorldInstanceSubsystem->GetCurrentLevelName().Contains("Level1"))
	{
		WorldInstanceSubsystem->SetCurrentLevelDisplayName(FText::FromString("Level 1"));
	}
	else if (WorldInstanceSubsystem->GetCurrentLevelName().Contains("Level 2"))
	{
		WorldInstanceSubsystem->SetCurrentLevelDisplayName(FText::FromString("Level 2"));
	}
}

void ABaseGameState::InitializeSoundSubsystem()
{
	SoundSubsystem = GetGameInstance()->GetSubsystem<USoundInstanceSubsystem>();
	check(SoundSubsystem);
	SoundSubsystem->SetWorldCache(World);
	SoundSubsystem->LoadSoundClass();
	SoundSubsystem->LoadVolumeSettings();
}

void ABaseGameState::InitializeWorldActorLoader()
{
	//새 게임 시작 시, 경량 액터 초기화
	if (!bIsNewGame)
	{
		return;
	}

	WorldActorLoaderSubsystem = World->GetSubsystem<UWorldActorLoaderSubsystem>();
	check(WorldActorLoaderSubsystem);

	WorldActorLoaderSubsystem->BindDelegateToWorldActorLoader();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ALightWeightActor::StaticClass(), FoundActors);

	if (FoundActors.Num() == 0)
	{
		Debug::Print(TEXT("ABaseGameState::InitializeWorldActorLoader : No LightWeightActors found in the World"));

		//크래시 방지(경량 액터가 존재하지 않을 시 바인딩 제거하지 않으면 크래시발생)
		WorldInstanceSubsystem->OnRecentCrackChanged.Clear();
		return;
	}
	
	TMap<int32, FRegionActorData> NewRegionDataMap;
	
	for (AActor* Actor : FoundActors)
	{
		ALightWeightActor* LightActor = Cast<ALightWeightActor>(Actor);
		if (!IsValid(LightActor))
		{
			Debug::Print(FString::Printf(TEXT("LightWeightActor is invalid : %s"), *LightActor->GetName()));
			continue;
		}

		AllLightWeightActorsCache.Add(LightActor);

		int32 RegionIndex = LightActor->GetLocatedRegionIndex();
		EWorldActorCategory Category = LightActor->GetActorCategory();

		if (!NewRegionDataMap.Contains(RegionIndex))
		{
			FRegionActorData NewRegionActorData;
			NewRegionActorData.ActorCategory = Category;
			NewRegionDataMap.Add(RegionIndex, NewRegionActorData);
		}
		else
		{
			NewRegionDataMap[RegionIndex].ActorCategory = Category;
		}

		FWorldActorData NewActorData;
		NewActorData.InstanceGuid = LightActor->GetUniqueGuid();
		NewActorData.LinkedActorClass = LightActor->GetLinkedActorClass();
		NewActorData.bIsActivate = false;
		NewActorData.Location = LightActor->GetActorLocation();
		NewActorData.Rotation = LightActor->GetActorRotation();

		switch (Category)
		{
		case EWorldActorCategory::None:
			break;
		case EWorldActorCategory::Item:
			NewActorData.bIsCollected = false;
			break;
		case EWorldActorCategory::Interactable:
			NewActorData.bIsInteracted = false;
			break;
		case EWorldActorCategory::Enemy:
			NewActorData.CurrentHealth = 100.f;
			break;
		default:
			break;
		}
		NewRegionDataMap[RegionIndex].WorldActorArray.Add(NewActorData);
	}
	int32 MaxRegion = WorldInstanceSubsystem->GetCrackDataMap()->Find(WorldInstanceSubsystem->GetCurrentLevelName())->CrackDataArray.Last().CrackIndex;
	WorldActorLoaderSubsystem->SetMaxRegion(MaxRegion);
	
	TArray<bool> NewActivateRegionArray;
	NewActivateRegionArray.SetNumZeroed(MaxRegion + 1);
	WorldActorLoaderSubsystem->SetActivateRegionArray(NewActivateRegionArray);
	
	WorldActorLoaderSubsystem->SetRegionDataMap(NewRegionDataMap);

	//초기 스폰 Update 및 실행
	WorldActorLoaderSubsystem->UpdateActivateRegions(0);
	WorldActorLoaderSubsystem->ActiveRegion(0);
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

void ABaseGameState::LoadCrackDataFromInstance()
{
	for (int32 i = 0; i < AllCracksCache.Num(); i++)
	{
		ACrack* Crack = Cast<ACrack>(AllCracksCache[i]);

		const FCrackData* CrackData = WorldInstanceSubsystem->GetCrackData(
			WorldInstanceSubsystem->GetCurrentLevelName(), i);
		if (CrackData && CrackData->bIsActivate)
		{
			Crack->SetActive();
			Crack->SetCrackName(CrackData->CrackName);
		}
	}
}

void ABaseGameState::InitializeCrackDataMap()
{
	// 불러오지 않은 새 게임일 경우에만 실행
	if (!bIsNewGame)
	{
		return;
	}

	if (AllCracksCache.IsEmpty())
	{
		Debug::Print(TEXT("No Cracks in World"));
		return;
	}
	
	TMap<FString, FCrackDataArrayStruct>* CrackDataMap = WorldInstanceSubsystem->GetCrackDataMap();
	const FString& CurrentLevelName = WorldInstanceSubsystem->GetCurrentLevelName();
	FCrackDataArrayStruct& CrackDataArray = (*CrackDataMap)[CurrentLevelName];
	
	for (ACrack* Crack : AllCracksCache)
	{
		if (Crack->GetCrackIndex() == 0)
		{
			continue;
		}

		FCrackData NewCrackData;

		NewCrackData.OwnerLevelName = WorldInstanceSubsystem->GetCurrentLevelName();
		NewCrackData.CrackIndex = Crack->GetCrackIndex();
		NewCrackData.CrackName = Crack->GetCrackName();
		NewCrackData.bIsActivate = Crack->GetIsActivate();
		NewCrackData.RespawnLocation = Crack->GetRespawnTargetPointLocation();
		NewCrackData.RespawnRotation = Crack->GetRespawnTargetPointRotation();

		bool bAlreadyExists = false;
		for (const FCrackData& ExistData : CrackDataArray.CrackDataArray)
		{
			if (ExistData.CrackIndex == Crack->GetCrackIndex())
			{
				bAlreadyExists = true;
				break;
			}
		}
		if (!bAlreadyExists)
		{
			// 해당 인덱스 존재하지 않을 때만 데이터 추가
			CrackDataArray.CrackDataArray.Add(NewCrackData);
		}
	}
}

void ABaseGameState::InitializeZeroIndexCrackData(const FString CurrentLevelName)
{
	check(CrackInitializeDataTable);

	if (IsValid(WorldInstanceSubsystem))
	{
		if (WorldInstanceSubsystem->GetCurrentLevelName() != "Proto_Level1" &&
			WorldInstanceSubsystem->GetCurrentLevelName() != "Proto_Level2" &&
			WorldInstanceSubsystem->GetCurrentLevelName() != "TestCrackLevel1" &&
			WorldInstanceSubsystem->GetCurrentLevelName() != "TestCrackLevel2" &&
			WorldInstanceSubsystem->GetCurrentLevelName() != "PastLevel" &&
			WorldInstanceSubsystem->GetCurrentLevelName() != "PresentLevel")
		{
			return;
		}
	}
	
	FCrackInitializeData* Level1Row = CrackInitializeDataTable->FindRow<FCrackInitializeData>(FName(CurrentLevelName), TEXT(""));
	FCrackInitializeData* Level2Row = CrackInitializeDataTable->FindRow<FCrackInitializeData>(FName(Level1Row->ZeroIndexCrackData.LinkedLevelName), TEXT(""));
	FCrackData Level1 = Level1Row->ZeroIndexCrackData;
	FCrackData Level2 = Level2Row->ZeroIndexCrackData;

	if (IsValid(WorldInstanceSubsystem))
	{
		WorldInstanceSubsystem->InitializeCrackDataMap(Level1, Level2);
	}
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
