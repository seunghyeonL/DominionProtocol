// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/SkeletonEnemy.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"

ASkeletonEnemy::ASkeletonEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	PawnTag = PawnTags::AxeSkeleton;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("hand_r"));

	WeaponTraceBox = CreateDefaultSubobject<UMnhBoxComponent>(TEXT("WeaponTraceBox"));
	WeaponTraceBox->SetupAttachment(WeaponMesh);

	AttackTraceComponent = CreateDefaultSubobject<UMnhTracerComponent>(TEXT("AttackTraceComponent"));

	FMnhTracerConfig TracerConfig;
	TracerConfig.TracerTag = ItemTags::AxeWeapon;
	TracerConfig.DrawDebugType = EDrawDebugTrace::None;
	TracerConfig.DebugDrawTime = 1.f;
	TracerConfig.TraceSettings.TraceChannel = ECC_Pawn;

	AttackTraceComponent->TracerConfigs.Add(TracerConfig);

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(ItemTags::AxeWeapon);

	AttackTraceComponent->InitializeTracers(TagContainer, WeaponTraceBox);
}

void ASkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void ASkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASkeletonEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
