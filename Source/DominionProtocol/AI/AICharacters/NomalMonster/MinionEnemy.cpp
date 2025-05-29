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
	LeftWeaponTraceBox->SetupAttachment(GetMesh(), TEXT("hand_l"));

	LeftAttackTraceComponent = CreateDefaultSubobject<UMnhTracerComponent>(TEXT("LeftAttackTraceComponent"));

	FMnhTracerConfig LeftTracerConfig;
	LeftTracerConfig.TracerTag = ItemTags::SwordWeapon;
	LeftTracerConfig.TraceSettings.TraceChannel = ECC_Pawn;
	LeftTracerConfig.DrawDebugType = EDrawDebugTrace::None;

	LeftAttackTraceComponent->TracerConfigs.Add(LeftTracerConfig);
	LeftAttackTraceComponent->InitializeTracers(FGameplayTagContainer({ ItemTags::SwordWeapon }), LeftWeaponTraceBox);

	// 오른손 무기 트레이스 박스
	RightWeaponTraceBox = CreateDefaultSubobject<UMnhBoxComponent>(TEXT("RightWeaponTraceBox"));
	RightWeaponTraceBox->SetupAttachment(GetMesh(), TEXT("hand_r"));

	RightAttackTraceComponent = CreateDefaultSubobject<UMnhTracerComponent>(TEXT("RightAttackTraceComponent"));

	FMnhTracerConfig RightTracerConfig;
	RightTracerConfig.TracerTag = ItemTags::SwordWeapon;
	RightTracerConfig.TraceSettings.TraceChannel = ECC_Pawn;
	RightTracerConfig.DrawDebugType = EDrawDebugTrace::None;

	RightAttackTraceComponent->TracerConfigs.Add(RightTracerConfig);
	RightAttackTraceComponent->InitializeTracers(FGameplayTagContainer({ ItemTags::SwordWeapon }), RightWeaponTraceBox);
}

void AMinionEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitializeStatusComponent();
	InitializeSkillComponent();
}