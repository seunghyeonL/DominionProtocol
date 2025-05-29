// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/MinionEnemy.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"

AMinionEnemy::AMinionEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	PawnTag = PawnTags::Minion;

	// 왼손 무기 트레이스 박스
	LeftWeaponTraceBox = CreateDefaultSubobject<UMnhBoxComponent>(TEXT("LeftWeaponTraceBox"));
	LeftWeaponTraceBox->SetupAttachment(GetMesh(), TEXT("weapon_sword_l"));

	// 오른손 무기 트레이스 박스
	RightWeaponTraceBox = CreateDefaultSubobject<UMnhBoxComponent>(TEXT("RightWeaponTraceBox"));
	RightWeaponTraceBox->SetupAttachment(GetMesh(), TEXT("weapon_sword_r"));

	AttackTraceComponent = CreateDefaultSubobject<UMnhTracerComponent>(TEXT("RightAttackTraceComponent"));

	FMnhTracerConfig TracerConfig;
	TracerConfig.TracerTag = ItemTags::BasicWeapon;
	TracerConfig.TraceSettings.TraceChannel = ECC_Pawn;
	TracerConfig.DrawDebugType = EDrawDebugTrace::ForDuration;

	AttackTraceComponent->TracerConfigs.Add(TracerConfig);

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(ItemTags::BasicWeapon);

	TArray<UMnhBoxComponent*> TraceBoxes = { LeftWeaponTraceBox, RightWeaponTraceBox };
	for (UMnhBoxComponent* Box : TraceBoxes)
	{
		AttackTraceComponent->InitializeTracers(TagContainer, Box);
	}
}

void AMinionEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitializeStatusComponent();
	InitializeSkillComponent();
}