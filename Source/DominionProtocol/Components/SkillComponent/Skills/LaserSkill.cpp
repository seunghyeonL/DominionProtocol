// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/LaserSkill.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "GameFramework/Character.h"
#include "Util/GameTagList.h"

ULaserSkill::ULaserSkill()
{
	SkillTag = SkillTags::LaserSkill;
}

void ULaserSkill::Initialize(ACharacter* Owner)
{
	Super::Initialize(Owner);

	UMnhTracerComponent* TracerComponent = OwnerCharacter->FindComponentByClass<UMnhTracerComponent>();

	if (!IsValid(TracerComponent))
	{
		return;
	}

	// TraceBox
	// 컴포넌트 생성
	CapsuleComponent = NewObject<UMnhCapsuleComponent>(OwnerCharacter, TEXT("MagicTraceBox"));

	// 액터의 메시에 부착
	CapsuleComponent->AttachToComponent(OwnerCharacter->GetMesh(),FAttachmentTransformRules::KeepRelativeTransform);
	
	// 컴포넌트 등록 (필수!)
	CapsuleComponent->RegisterComponent();

	FMnhTracerConfig TracerConfig;

	TracerConfig.TracerTag = ItemTags::BasicWeapon;
	TracerConfig.DrawDebugType = EDrawDebugTrace::ForDuration;
	TracerConfig.DebugDrawTime = 2.f;
	TracerConfig.TraceSettings.TraceChannel = ECC_Pawn;

	TracerComponent->TracerConfigs.Add(TracerConfig);

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(SkillTag);

	TracerComponent->InitializeTracers(TagContainer, CapsuleComponent);
}

void ULaserSkill::BeginDestroy()
{
	if (IsValid(CapsuleComponent))
	{
		CapsuleComponent->DestroyComponent();
		CapsuleComponent = nullptr;
	}

	Super::BeginDestroy();
}
