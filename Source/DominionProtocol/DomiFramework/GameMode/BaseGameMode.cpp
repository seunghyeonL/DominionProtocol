// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

#include "AI/AICharacters/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/WorldInstanceSubsystem.h"
#include "DomiFramework/GameInstance/ItemInstanceSubsystem.h"
#include "Interface/StoryDependentInterface.h"
#include "Player/Characters/DomiCharacter.h"
#include "WorldObjects/Crack.h"
#include "WorldObjects/DropEssence.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "EnumAndStruct/FCrackData.h"
#include "EngineUtils.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "MovieScene.h"
#include "MovieSceneSequence.h"
#include "Camera/CameraComponent.h"
#include "Engine/Engine.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"
#include "GameFramework/SpringArmComponent.h"

#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"

ABaseGameMode::ABaseGameMode()
	:	GameInstance(nullptr),
		PlayerCharacter(nullptr),
		RecentCrackCache(nullptr),
		RespawnDelay(2.f),
		PlayTime(0),
		bIsFadeIn(true)
{
	static ConstructorHelpers::FClassFinder<ADropEssence> DropEssenceBPClass(TEXT("/Game/WorldObjects/BP_DropEssence"));
	if (DropEssenceBPClass.Succeeded())
	{
		DropEssenceClass = DropEssenceBPClass.Class;
	}

	EnterAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EnterAudioComponent"));
	EnterAudioComponent->SetSound(EnterSound);
	ExitAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ExitAudioComponent"));
	ExitAudioComponent->SetSound(ExitSound);
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	checkf(GameInstance, TEXT("Get GameInstance Fail"));

	WorldInstanceSubsystem = GameInstance->GetSubsystem<UWorldInstanceSubsystem>();
	check(WorldInstanceSubsystem);

	ItemInstanceSubsystem = GameInstance->GetSubsystem<UItemInstanceSubsystem>();
	check(ItemInstanceSubsystem);

	World = GetWorld();
	check(IsValid(World));
	
	if (!IsValid(FadeSequence))
	{
		FadeSequence = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/Levels/LevelSequence/FadeTrack"));
		if (!IsValid(FadeSequence))
		{
			Debug::Print(FString::Printf(TEXT("%s::BeginPlay : Failed to load 'FadeTrack' Level Sequence"), *GetName()));
			return;
		}
	}
	
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), 
		FadeSequence, 
		FMovieSceneSequencePlaybackSettings(),
		SequenceActor
	);

	if (IsValid(SequencePlayer))
	{
		SequencePlayer->SetPlayRate(1.0f);
		SequencePlayer->OnFinished.AddDynamic(this, &ABaseGameMode::OnFadeSequenceFinished);
	}
}

void ABaseGameMode::StartPlay()
{
	Super::StartPlay();

	PlayerCharacter = Cast<ADomiCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (IsValid(PlayerCharacter) && !PlayerCharacter->ActorHasTag("Player"))
	{
		PlayerCharacter->Tags.Add("Player");
	}
	
	BaseGameState = Cast<ABaseGameState>(World->GetGameState());
	check(GameState);
	BaseGameState->InitializeGame();
	
	if (WorldInstanceSubsystem->GetIsLevelChanged())
	{
		Debug::Print(FString::Printf(TEXT("%s, %s"), *WorldInstanceSubsystem->GetMoveTargetLocation().ToString(), *WorldInstanceSubsystem->GetMoveTargetRotation().ToString()));
		PlayerCharacter->SetActorLocationAndRotation(WorldInstanceSubsystem->GetMoveTargetLocation(), WorldInstanceSubsystem->GetMoveTargetRotation());
		WorldInstanceSubsystem->SwitchIsLevelChanged();
		WorldInstanceSubsystem->SetMoveTargetLocation(FVector::ZeroVector);
		WorldInstanceSubsystem->SetMoveTargetRotator(FRotator::ZeroRotator);
		AController* PlayerController = PlayerCharacter->GetController();
		FRotator NewRotation = PlayerCharacter->GetActorForwardVector().Rotation();
		PlayerController->SetControlRotation(NewRotation);
	}
	
	//=====Enemy 위치정보 캐싱=====
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), Enemies);
	for (AActor* Enemy : Enemies)
	{
		//FGameplayTag 정해지면 그거로 바꿔야할 듯?
		if (Enemy->GetName().Contains("Boss")) continue;

		FEnemySpawnInfo Info;
		Info.EnemyClass = Enemy->GetClass();
		Info.OriginTransform = Enemy->GetActorTransform();

		CachedEnemyInfo.Add(Info);
	}
	//==========================

	// 스토리 상태 불러오기
	Debug::Print(FString::Printf(TEXT("Current Story State: %s"), *UEnum::GetValueAsString(GameInstance->GetCurrentGameStoryState())));
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor->GetClass()->ImplementsInterface(UStoryDependentInterface::StaticClass()))
		{
			IStoryDependentInterface::Execute_OnStoryStateUpdated(Actor, GameInstance->GetCurrentGameStoryState());
		}
	}

	ExitAudioComponent->Play();

	World->GetTimerManager().SetTimer(
		PlayTimer,
		this,
		&ABaseGameMode::PlayTimeAdder,
		60.f,
		true);
	
	// 페이드인
	SetPlayerInputEnable(false);
	PlayFade(true);
}

void ABaseGameMode::StartBattle(AActor* SpawnedBoss)
{
	OnStartBattle.Broadcast(SpawnedBoss);
}

void ABaseGameMode::EndBattle()
{
	OnEndBattle.Broadcast();
}

void ABaseGameMode::Save()
{
	USaveManagerSubsystem* SaveManagerSubsystem = GameInstance->GetSubsystem<USaveManagerSubsystem>();
	check(IsValid(SaveManagerSubsystem));
	
	FSaveSlotMetaData NewSaveSlotMetaData;
	NewSaveSlotMetaData.SaveSlotExist = true;
	NewSaveSlotMetaData.SaveSlotName = GameInstance->GetSaveSlotName();
	NewSaveSlotMetaData.SaveSlotIndex = GameInstance->GetSaveSlotIndex();
	NewSaveSlotMetaData.SaveDateTime = FDateTime::Now();
	NewSaveSlotMetaData.PlayTime = PlayTime;
	NewSaveSlotMetaData.PlayingLevelName = WorldInstanceSubsystem->GetCurrentLevelName();
	NewSaveSlotMetaData.PlayingLevelDisplayName = WorldInstanceSubsystem->GetCurrentLevelDisplayName();
	NewSaveSlotMetaData.RecentCrackName = RecentCrackCache->GetCrackName();
	NewSaveSlotMetaData.PlayerLevel = static_cast<int32>(PlayerCharacter->GetStatusComponent()->GetStat(StatTags::Level));

	SaveManagerSubsystem->SetSaveSlotData(NewSaveSlotMetaData.SaveSlotIndex, NewSaveSlotMetaData);
	SaveManagerSubsystem->SaveSettings();
	SaveManagerSubsystem->SaveGame(GameInstance->GetSaveSlotName(), GameInstance->GetSaveSlotIndex());
}

void ABaseGameMode::OnPlayerDeath()
{
	checkf(RecentCrackCache, TEXT("ABaseGameMode::OnPlayerDeath : RecentCrackCache is Nullptr"));
	
	Debug::Print("ABaseGameMode::OnPlayerDeath : Respawn Player");

	if (UDomiGameInstance* GI = GetGameInstance<UDomiGameInstance>())
	{
		if (GI->ReturnStoryState())
		{
			EndBattle();
		}
	}
	
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(
		RespawnTimerHandle,
		this,
		&ABaseGameMode::RespawnPlayerCharacter,
		RespawnDelay,
		false);
}

void ABaseGameMode::RestorePlayer()
{
	//체력회복
	StatusComponent = PlayerCharacter->GetStatusComponent();
	if (IsValid(StatusComponent))
	{
		StatusComponent->SetHealth(UE_BIG_NUMBER);
	}
		
	//회복 포션 개수 -> Max 회복
	ItemComponent = PlayerCharacter->GetItemComponent();
	if (IsValid(ItemComponent))
	{
		ItemComponent->RestorePotion();
	}
}

void ABaseGameMode::RespawnPlayerCharacter()
{
	DestroyAllNormalEnemy();

	// Essence 레벨에 드랍
	ADropEssence* DropEssence = WorldInstanceSubsystem->GetDropEssenceCache();
	if (IsValid(DropEssence))
	{
		DropEssence->Destroy();
		WorldInstanceSubsystem->SetDropEssenceCache(nullptr);
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ADropEssence* NewDropEssence = World->SpawnActor<ADropEssence>(DropEssenceClass, PlayerCharacter->GetActorLocation(), PlayerCharacter->GetActorRotation(), SpawnParams);
	if (IsValid(NewDropEssence))
	{
		Debug::Print(FString::Printf(TEXT("Spawned DropEssence : %s"), *NewDropEssence->GetName()));
		WorldInstanceSubsystem->SetDropEssenceCache(NewDropEssence);
		WorldInstanceSubsystem->SetIsDropEssenceExist(true);
		WorldInstanceSubsystem->SetDropEssenceAmount(GameInstance->GetPlayerCurrentEssence());
		WorldInstanceSubsystem->SetDropEssenceLocation(PlayerCharacter->GetActorLocation());
		WorldInstanceSubsystem->SetDropEssenceLocationLevel(WorldInstanceSubsystem->GetCurrentLevelName());
		GameInstance->SetPlayerCurrentEssence(0);
	}

	// 플레이어 리스폰
	if (IsValid(PlayerCharacter))
	{
		FVector RespawnLocation = RecentCrackCache->GetRespawnTargetPointLocation();
		FRotator RespawnRotation = RecentCrackCache->GetRespawnTargetPointRotation();
		
		PlayerCharacter->SetActorLocation(RespawnLocation);
		PlayerCharacter->SetActorRotation(RespawnRotation);

		//일단은 체력회복하고 EffectTags::Death 상태 해제
		if (!IsValid(StatusComponent))
		{
			StatusComponent = PlayerCharacter->GetStatusComponent();
		}
		StatusComponent->SetHealth(UE_BIG_NUMBER);
		TObjectPtr<UPlayerControlComponent> PlayerControlComponent = PlayerCharacter->GetPlayerControlComponent();
		PlayerControlComponent->DeactivateControlEffect(EffectTags::Death);
		RestorePlayer();
		
		ExitAudioComponent->Play();
		
		// Using InGameHUD
		OnPlayerSpawn.Broadcast();

		UpdateInstanceData();
		Save();
		RespawnEnemies();
	}
}

void ABaseGameMode::MoveToTargetCrack(FString InOwningCrackLevelName, int32 InCrackIndex)
{
	if (!WorldInstanceSubsystem->GetIsActivateCrackIndex(InOwningCrackLevelName, InCrackIndex))
	{
		Debug::Print(TEXT("Crack is not Activate"));
		return;
	}
	
	const FString& CurrentLevelName = WorldInstanceSubsystem->GetCurrentLevelName();
	const FCrackData* TargetCrackData = WorldInstanceSubsystem->GetCrackData(InOwningCrackLevelName, InCrackIndex);

	if (!TargetCrackData)
	{
		Debug::Print(TEXT("TargetCrackData is nullptr"));
		return;
	}
	
	// Debug::Print(FString::Printf(TEXT("Target Location: X=%.2f, Y=%.2f, Z=%.2f"), 
	// 							TargetCrackData->RespawnLocation.X,
	// 							TargetCrackData->RespawnLocation.Y,
	// 							TargetCrackData->RespawnLocation.Z));
	
	WorldInstanceSubsystem->SetRecentCrackIndex(InCrackIndex);
	WorldInstanceSubsystem->SetRecentCrackName(TargetCrackData->CrackName);
	WorldInstanceSubsystem->SetMoveTargetLocation(TargetCrackData->RespawnLocation);
	WorldInstanceSubsystem->SetMoveTargetRotator(TargetCrackData->RespawnRotation);

	PlayFade(false);
	if (CurrentLevelName == InOwningCrackLevelName)
	{
		bIsSameLevelMove = true;
		PendingMoveLocation = TargetCrackData->RespawnLocation;
		PendingMoveRotation = TargetCrackData->RespawnRotation;
		RecentCrackCache = BaseGameState->GetCrackByIndex(InCrackIndex);
	}
	else if (CurrentLevelName != InOwningCrackLevelName)
	{
		bIsSameLevelMove = false;
		MoveTargetLevelName = FName(InOwningCrackLevelName);
		Debug::Print(TEXT("Move Another Level"));
		WorldInstanceSubsystem->SwitchIsLevelChanged();
	}
}

void ABaseGameMode::DestroyAllNormalEnemy()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), Enemies);
	for (AActor* Enemy : Enemies)
	{
		Enemy->Destroy();
	}
}

void ABaseGameMode::RespawnEnemies()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (FEnemySpawnInfo EnemyInfo : CachedEnemyInfo)
	{
		ABaseEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ABaseEnemy>(
			EnemyInfo.EnemyClass,
			EnemyInfo.OriginTransform,
			SpawnParams);
	}
}



#pragma region KyuHyeok

void ABaseGameMode::PlayFade(bool bFadeIn)
{
	if (!IsValid(FadeSequence))
	{
		Debug::Print(TEXT("FadeSequence is not valid"));
		return;
	}

	if (!IsValid(SequencePlayer))
	{
		Debug::Print(TEXT("SequencePlayer is not valid"));
		return;
	}

	FFrameRate FrameRate = FadeSequence->GetMovieScene()->GetTickResolution();
	FFrameTime SequenceLength = FadeSequence->GetMovieScene()->GetPlaybackRange().Size<FFrameTime>();
	
	if (bFadeIn)
	{
		bIsFadeIn = true;
		SequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(FFrameTime(0), EUpdatePositionMethod::Play));
		SequencePlayer->SetPlayRate(1.f);
		SequencePlayer->Play();
		EnterAudioComponent->Play();
	}
	else
	{
		bIsFadeIn = false;
		SetPlayerInputEnable(false);
		SequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(SequenceLength, EUpdatePositionMethod::Play));
		SequencePlayer->SetPlayRate(-1.f);
		SequencePlayer->Play();
		ExitAudioComponent->Play();
	}
}

void ABaseGameMode::PlayerLevelUp(FGameplayTag StatTag)
{
	if (IsValid(PlayerCharacter))
	{
		if (!IsValid(StatusComponent))
		{
			StatusComponent =  PlayerCharacter->GetStatusComponent();
		}
		//레벨업 로직 추후 작성
	}
}

void ABaseGameMode::OnFadeSequenceFinished()
{
	if (bIsFadeIn)
	{
		bIsFadeIn = false;
		SetPlayerInputEnable(true);
	}
	else
	{
		if (bIsSameLevelMove)
		{
			PlayerCharacter->SetActorLocationAndRotation(PendingMoveLocation, PendingMoveRotation);
			AController* PlayerController = PlayerCharacter->GetController();
			FRotator NewRotation = PlayerCharacter->GetActorForwardVector().Rotation();
			PlayerController->SetControlRotation(NewRotation);
			PlayFade(true);
			ExitAudioComponent->Play();
		}
		else
		{
			UGameplayStatics::OpenLevel(this, FName(MoveTargetLevelName));
		}
	}
}

void ABaseGameMode::SaveItemDataToInstance()
{
	if (IsValid(PlayerCharacter))
	{
		if (!IsValid(ItemComponent))
		{
			ItemComponent =  PlayerCharacter->GetItemComponent();
		}
		ItemComponent->UpdateItemInstanceSubsystem();
	}
}

void ABaseGameMode::UpdateInstanceData()
{
	// 플레이어 스탯 인스턴스 전달
	GameInstance->SetStatDataMap(PlayerCharacter->GetStatusComponent()->GetStatMap());
	// 플레이어 인벤토리 인스턴스 전달
	SaveItemDataToInstance();
	// 플레이타임 인스턴스 전달
	GameInstance->AddPlayTime(GetPlayTime());
}

void ABaseGameMode::PlayTimeAdder()
{
	PlayTime++;
}

void ABaseGameMode::SetPlayerInputEnable(bool bEnable)
{
	if (!IsValid(PlayerCharacter))
	{
		Debug::Print(TEXT("PlayerCharacter is not valid"));
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
	if (!IsValid(PlayerController))
	{
		Debug::Print(TEXT("PlayerController is not valid"));
		return;
	}
	
	if (bEnable)
	{
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->EnableInput(PlayerController);

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
	else
	{
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->DisableInput(PlayerController);

		FInputModeUIOnly InputMode;
		PlayerController->SetInputMode(InputMode);
	}
	
}

#pragma endregion


#pragma region SeoYoung



#pragma endregion