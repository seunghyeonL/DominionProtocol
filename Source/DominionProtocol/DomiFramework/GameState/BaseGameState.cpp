// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameState.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "DomiFramework/GameInstance/SoundInstanceSubsystem.h"
#include "Kismet/GameplayStatics.h"

#include "Util/DebugHelper.h"

ABaseGameState::ABaseGameState()
	:	GameInstance(nullptr),
		SoundSubsystem(nullptr)
{
}

void ABaseGameState::BeginPlay()
{
	Super::BeginPlay();

	InitializeGameInstance();
	InitializeSoundSubsystem();
}

void ABaseGameState::InitializeGameInstance()
{
	GameInstance = Cast<UDomiGameInstance>(GetGameInstance());
	check(GameInstance);
	GameInstance->SetCurrentLevelName(UGameplayStatics::GetCurrentLevelName(GetWorld(), true));
	if (GameInstance->GetCurrentLevelName().Contains("Level1"))
	{
		GameInstance->SetCurrentLevelDisplayName(FText::FromString("Level 1"));
	}
	else if (GameInstance->GetCurrentLevelName().Contains("Level 2"))
	{
		GameInstance->SetCurrentLevelDisplayName(FText::FromString("Level 2"));
	}
}

void ABaseGameState::InitializeSoundSubsystem()
{
	SoundSubsystem = GetGameInstance()->GetSubsystem<USoundInstanceSubsystem>();
	check(SoundSubsystem);
	SoundSubsystem->SetWorldCache(GetWorld());
	SoundSubsystem->LoadVolumeSettings();
}

FSkillData* ABaseGameState::GetSkillData(const FGameplayTag SkillTag) const
{
	check(SkillDataTable);
	
	return SkillDataTable->FindRow<FSkillData>(SkillTag.GetTagName(), TEXT(""));
}