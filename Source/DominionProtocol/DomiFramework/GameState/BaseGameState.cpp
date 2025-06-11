// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"

#include "Components/ItemComponent/ItemComponent.h"
#include "GameFramework/Character.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"
#include "DomiFramework/GameInstance/ItemInstanceSubsystem.h"
#include "Components/SkillComponent/SkillComponentInitializeData.h"
#include "Components/StatusComponent/StatusComponentInitializeData.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "EnumAndStruct/EffectData/EffectInitializeData.h"
#include "EnumAndStruct/FCrackData.h"
#include "WorldObjects/Crack.h"
#include "Player/Characters/DomiCharacter.h"
#include "Kismet/GameplayStatics.h"

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
	InitializeItemInstanceSubsystem();
}

void ABaseGameState::InitializeGameInstance()
{
	GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	check(IsValid(GameInstance));
	GameInstance->SetWorldCache(World);
}

void ABaseGameState::InitializeWorldInstanceSubsystem()
{
	WorldInstanceSubsystem = GameInstance->GetSubsystem<UWorldInstanceSubsystem>();
	check(IsValid(WorldInstanceSubsystem));

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
	check(IsValid(SoundSubsystem));
	SoundSubsystem->SetWorldCache(World);
	SoundSubsystem->LoadSoundClass();
	SoundSubsystem->LoadVolumeSettings();
}

void ABaseGameState::InitializeItemInstanceSubsystem()
{
	ItemInstanceSubsystem = GetGameInstance()->GetSubsystem<UItemInstanceSubsystem>();
	check(IsValid(ItemInstanceSubsystem));
	ItemInstanceSubsystem->SetWorldCache(World);
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

FEffectInitializeData* ABaseGameState::GetEffectInitializeData(const FGameplayTag EffectTag) const
{
	check(EffectInitializeDataTable);

	return EffectInitializeDataTable->FindRow<FEffectInitializeData>(EffectTag.GetTagName(), TEXT(""));
}

void ABaseGameState::InitializeGame()
{
	check(IsValid(GameInstance));
	check(IsValid(WorldInstanceSubsystem));
	check(IsValid(SoundSubsystem));
	check(IsValid(ItemInstanceSubsystem));
	
	CacheAllCracks();
	LoadCrackDataFromInstance();
	InitializeCrackDataMap();
	BaseGameMode = Cast<ABaseGameMode>(World->GetAuthGameMode());
	if (IsValid(BaseGameMode))
	{
		BaseGameMode->SetRecentCrackCache(FindNearestCrack());
	}
	LoadItemDataFromInstance();

	// 캐릭터 스탯값 적용
	if (IsValid(World))
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(World->GetFirstPlayerController()->GetPawn());
		if (IsValid(PlayerCharacter))
		{
			UStatusComponent* StatusComponent = PlayerCharacter->GetStatusComponent();
			if (IsValid(StatusComponent))
			{
				if (!GameInstance->GetStatDataMap().IsEmpty())
				{
					StatusComponent->SetStatMap(GameInstance->GetStatDataMap());
				}
			}
		}
	}

	// DropEssence 인스턴스 상 존재 하면 레벨에 적용
	if (WorldInstanceSubsystem->GetIsDropEssenceExist() && WorldInstanceSubsystem->GetDropEssenceLocationLevel() == WorldInstanceSubsystem->GetCurrentLevelName())
	{
		TSubclassOf<ADropEssence> DropEssenceClass;
		static ConstructorHelpers::FClassFinder<ADropEssence> DropEssenceBPClass(TEXT("/Game/WorldObjects/BP_DropEssence"));
		if (DropEssenceBPClass.Succeeded())
		{
			DropEssenceClass = DropEssenceBPClass.Class;
		}
		
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(World->GetFirstPlayerController()->GetPawn());
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ADropEssence* NewDropEssence = World->SpawnActor<ADropEssence>(DropEssenceClass, WorldInstanceSubsystem->GetDropEssenceLocation(), FRotator::ZeroRotator, SpawnParams);
	}
	else
	{
		if (WorldInstanceSubsystem->GetIsDropEssenceExist())
		{
			WorldInstanceSubsystem->SetDropEssenceCache(nullptr);
			WorldInstanceSubsystem->SetIsDropEssenceExist(false);
			WorldInstanceSubsystem->SetDropEssenceAmount(0);
			WorldInstanceSubsystem->SetDropEssenceLocation(FVector::ZeroVector);
			WorldInstanceSubsystem->SetDropEssenceLocationLevel("");
		}
	}
}

void ABaseGameState::CacheAllCracks()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ACrack::StaticClass(), FoundActors);

	if (FoundActors.IsEmpty())
	{
		return;
	}
	
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

void ABaseGameState::LoadItemDataFromInstance()
{
	if (!IsValid(ItemInstanceSubsystem))
	{
		Debug::PrintError(TEXT("ABaseGameState::LoadItemDataFromInstance: Invalid ItemInstanceSubsystem"));
		return;
	}
	
	if (!ItemInstanceSubsystem->GetInventoryDataMap().IsEmpty() &&
		!ItemInstanceSubsystem->GetEquipmentSlotMap().IsEmpty() &&
		!ItemInstanceSubsystem->GetConsumableSlotMap().IsEmpty())
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(World->GetFirstPlayerController()->GetPawn());
		if (IsValid(PlayerCharacter))
		{
			UItemComponent* ItemComponent = PlayerCharacter->GetItemComponent();
			if (IsValid(ItemComponent))
			{
				ItemComponent->SetInventoryMap(ItemInstanceSubsystem->GetInventoryDataMap());
				ItemComponent->SetEquipmentSlots(ItemInstanceSubsystem->GetEquipmentSlotMap());
				ItemComponent->SetConsumableSlots(ItemInstanceSubsystem->GetConsumableSlotMap());
				ItemComponent->SetIsPotionBoostApplied(ItemInstanceSubsystem->GetIsPotionBoostApplied());

				ItemComponent->SetSkillFromItemWhenLevelChanged();
				ItemComponent->DelegateExecuter();
			}
		}
	}
}

void ABaseGameState::InitializeCrackDataMap()
{
	// 불러오지 않은 새 게임일 경우에만 실행
	if (!WorldInstanceSubsystem->GetIsNewGame())
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
			WorldInstanceSubsystem->GetCurrentLevelName() != "PastLevel" &&
			WorldInstanceSubsystem->GetCurrentLevelName() != "PresentLevel")
		{
			return;
		}
	}

	if (!WorldInstanceSubsystem->GetIsNewGame())
	{
		return;
	}
	
	FCrackInitializeData* Level1Row = CrackInitializeDataTable->FindRow<FCrackInitializeData>(FName(CurrentLevelName), TEXT(""));
	FCrackInitializeData* Level2Row = CrackInitializeDataTable->FindRow<FCrackInitializeData>(FName(Level1Row->ZeroIndexCrackData.LinkedLevelName), TEXT(""));
	FCrackData Level1 = Level1Row->ZeroIndexCrackData;
	FCrackData Level2 = Level2Row->ZeroIndexCrackData;

	if (IsValid(WorldInstanceSubsystem))
	{
		WorldInstanceSubsystem->InitializeCrackAndNewGameDataMap(Level1, Level2);
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
