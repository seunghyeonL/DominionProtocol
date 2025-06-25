// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "BaseBossEnemy.h"

#include "Components/WidgetComponent/DomiWidgetComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABaseBossEnemy::ABaseBossEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BossID = 0; // 프로토타입 코드 - Default
	PawnTag = PawnTags::ProtoBoss;
}

// Called when the game starts or when spawned
void ABaseBossEnemy::BeginPlay()
{
	Super::BeginPlay();
	HPWidgetComponent->SetVisibility(false);
}

// Called every frame
void ABaseBossEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseBossEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseBossEnemy::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();
	UDomiGameInstance* GI = Cast<UDomiGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (GI)
	{
		GI->AdvanceStoryState();
	}
	if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->EndBattle(this);
	}
	//OnBossDeathDelegate.Broadcast();
}
