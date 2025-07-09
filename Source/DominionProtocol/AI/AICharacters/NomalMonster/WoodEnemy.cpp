// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/WoodEnemy.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"

AWoodEnemy::AWoodEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	PawnTag = PawnTags::Wood;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("foot_l"));

	WeaponTraceBox = CreateDefaultSubobject<UMnhBoxComponent>(TEXT("WeaponTraceBox"));
	WeaponTraceBox->SetupAttachment(WeaponMesh);

	AttackTraceComponent = CreateDefaultSubobject<UMnhTracerComponent>(TEXT("AttackTraceComponent"));

	FMnhTracerConfig TracerConfig;
	TracerConfig.TracerTag = ItemTags::BasicWeapon;
	TracerConfig.DrawDebugType = EDrawDebugTrace::None;
	TracerConfig.DebugDrawTime = 1.f;
	TracerConfig.TraceSettings.TraceChannel = ECC_Pawn;

	AttackTraceComponent->TracerConfigs.Add(TracerConfig);

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(ItemTags::BasicWeapon);

	AttackTraceComponent->InitializeTracers(TagContainer, WeaponTraceBox);

	EssenceAmount = 90.f;
}

void AWoodEnemy::Onewitch()
{
	if (AWoodEnemy* WoodEnemy = Cast<AWoodEnemy>(this))
	{
		FRotator CurrentRotation = GetActorRotation();
		CurrentRotation.Pitch = 0.f;
		CurrentRotation.Roll = 0.f;
		WoodEnemy->SetActorRotation(CurrentRotation);
	}
}