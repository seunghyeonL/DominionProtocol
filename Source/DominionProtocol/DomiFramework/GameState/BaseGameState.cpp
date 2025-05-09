// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"

#include "Util/DebugHelper.h"

ABaseGameState::ABaseGameState()
{
}

FSkillData* ABaseGameState::GetSkillData(const FGameplayTag SkillTag) const
{
	check(SkillDataTable);
	
	return SkillDataTable->FindRow<FSkillData>(SkillTag.GetTagName(), TEXT(""));
}

void ABaseGameState::BeginPlay()
{
	Super::BeginPlay();

	SoundSubsystem = GetGameInstance()->GetSubsystem<USoundInstanceSubsystem>();
	SoundSubsystem->SetWorldCache(GetWorld());
	SoundSubsystem->LoadVolumeSettings();
}
