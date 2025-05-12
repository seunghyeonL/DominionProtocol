// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameMode.h"

#include "AI/AICharacters/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Player/Characters/DomiCharacter.h"
#include "WorldObjects/Crack.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"

#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"

ABaseGameMode::ABaseGameMode()
	:	GameInstance(nullptr),
		PlayerCharacter(nullptr),
		RecentCrackCache(nullptr),
		RespawnDelay(2.f)
{/*
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	*/
}

void ABaseGameMode::BeginPlay()
{
	Super::BeginPlay();

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
}

void ABaseGameMode::StartPlay()
{
	Super::StartPlay();
	GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	checkf(GameInstance, TEXT("GI Fail"));

	PlayerCharacter = Cast<ADomiCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (IsValid(PlayerCharacter) && !PlayerCharacter->ActorHasTag("Player"))
	{
		PlayerCharacter->Tags.Add("Player");
	}
}

void ABaseGameMode::StartBattle()
{
}

void ABaseGameMode::EndBattle()
{
}

void ABaseGameMode::OnPlayerDeath()
{
	checkf(RecentCrackCache, TEXT("ABaseGameMode::OnPlayerDeath : RecentCrackCache is Nullptr"));
	
	Debug::Print("ABaseGameMode::OnPlayerDeath : Respawn Player");
	
	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(
		RespawnTimerHandle,
		this,
		&ABaseGameMode::RespawnPlayerCharacter,
		RespawnDelay,
		false);
}

void ABaseGameMode::RespawnPlayerCharacter()
{
	DestroyAllNormalEnemy();
	
	if (IsValid(PlayerCharacter))
	{
		FVector RespawnLocation = RecentCrackCache->GetRespawnTargetPointLocation();
		FRotator RespawnRotation = RecentCrackCache->GetRespawnTargetPointRotation();
		
		PlayerCharacter->SetActorLocation(RespawnLocation);
		PlayerCharacter->SetActorRotation(RespawnRotation);

		//일단은 체력회복하고 EffectTags::Death 상태 해제
		TObjectPtr<UStatusComponent> StatusComponent = PlayerCharacter->GetStatusComponent();
		StatusComponent->SetHealth(FLT_MAX);
		TObjectPtr<UPlayerControlComponent> PlayerControlComponent = PlayerCharacter->GetPlayerControlComponent();
		PlayerControlComponent->DeactivateControlEffect(EffectTags::Death);

		RespawnEnemies();
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