// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss1Enemy.h"

#include "Components/ItemComponent/ItemComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Player/Characters/DomiCharacter.h"

// Sets default values
ABoss1Enemy::ABoss1Enemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BossID = 1;
	PawnTag = PawnTags::Boss1;
}

void ABoss1Enemy::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

	SpawnDropItem();
}

// Called when the game starts or when spawned
void ABoss1Enemy::BeginPlay()
{
	Super::BeginPlay();

}

void ABoss1Enemy::SpawnDropItem()
{
	// 일반 DroppedItem 로직
	if (IsValid(DropItemClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		GetWorld()->SpawnActor<AItemDropped>(DropItemClass, GetGroundSpawnLocation(), GetActorRotation(), SpawnParams);
	}

	// 스토리 필수 아이템 자동 습득
	if (StoryItemTag.IsValid())
	{
		ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetPawn());
		if (IsValid(PlayerCharacter))
		{
			UItemComponent* ItemComponent = PlayerCharacter->FindComponentByClass<UItemComponent>();
			if (IsValid(ItemComponent))
			{
				ItemComponent->AddItem(StoryItemTag, 1);
			}
		}
	}
}

// Called every frame
void ABoss1Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABoss1Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABoss1Enemy::OnGroggy()
{
	// Super::OnGroggy();
	StatusComponent->ActivateStatusEffect(EffectTags::Groggy, 0.f, 8.f);
}
