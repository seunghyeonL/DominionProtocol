// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/BossMonsterHPBarWidget.h"

#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DomiFramework/GameMode/BaseGameMode.h"


void UBossMonsterHPBarWidget::UpdateBossMonsterMaxHP(const float NewMaxHP)
{
	MaxBossMonsterHP = NewMaxHP;
}

void UBossMonsterHPBarWidget::UpdateBossMonsterMaxShield(const float NewMaxShield)
{
	MaxBossMonsterShield = NewMaxShield;
}

void UBossMonsterHPBarWidget::UpdateBossMonsterHP(const float NewHP)
{
	AlphaForBossMonsterHPAnim = 0.0f;
	PreBossMonsterHP = CurrentBossMonsterHP;
	CurrentBossMonsterHP = NewHP;
}

void UBossMonsterHPBarWidget::UpdateBossMonsterShield(const float NewShield)
{
	AlphaForBossMonsterShieldAnim = 0.0f;
	PreBossMonsterShield = CurrentBossMonsterShield;
	CurrentBossMonsterShield = NewShield;
}

void UBossMonsterHPBarWidget::UpdateBossMonsterName(const FString NewBossMonsterName)
{
	BossMonsterName = NewBossMonsterName;

	OnUpdateBossMonsterName();
}


void UBossMonsterHPBarWidget::SpawnedBossMonster(AActor* NewMonster)
{
	if (BossMonsterName.IsEmpty())
	{
		if (NewMonster->GetClass()->ImplementsInterface(UPawnTagInterface::StaticClass()))
		{
			const FString CharacterName = IPawnTagInterface::Execute_GetPawnName(NewMonster);
			BossMonsterName = CharacterName;
		}
	}
	
	OnSpawnedBossMonster();
}

void UBossMonsterHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindBossSpawnedToWidgetDelegate();
}


void UBossMonsterHPBarWidget::BindBossSpawnedToWidgetDelegate()
{
	if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnBossSpawnedToWidget.AddUObject(this, &UBossMonsterHPBarWidget::BindSpawnedBossStatusDelegate);
		GM->OnBossSpawnedToWidget.AddUObject(this, &UBossMonsterHPBarWidget::SpawnedBossMonster);
	}
}

void UBossMonsterHPBarWidget::BindSpawnedBossStatusDelegate(AActor* SpawnedBoss)
{
	auto* BossEnemy = Cast<ABaseBossEnemy>(SpawnedBoss);
	if (BossEnemy)
	{
		auto* BossStatusComp = BossEnemy->FindComponentByClass<UStatusComponent>();
		if (BossStatusComp)
		{
			MaxBossMonsterHP = BossStatusComp->GetStat(StatTags::MaxHealth);
			CurrentBossMonsterHP = BossStatusComp->GetStat(StatTags::Health);
			MaxBossMonsterShield = BossStatusComp->GetStat(StatTags::MaxShield);
			CurrentBossMonsterShield = BossStatusComp->GetStat(StatTags::Shield);
			
			BossStatusComp->OnHealthChanged.AddDynamic(this, &UBossMonsterHPBarWidget::UpdateBossMonsterHP);
			BossStatusComp->OnMaxHealthChanged.AddDynamic(this, &UBossMonsterHPBarWidget::UpdateBossMonsterMaxHP);
			BossStatusComp->OnShieldChanged.AddDynamic(this, &UBossMonsterHPBarWidget::UpdateBossMonsterShield);
			BossStatusComp->OnMaxShieldChanged.AddUObject(this, &UBossMonsterHPBarWidget::UpdateBossMonsterMaxShield);
			BossStatusComp->OnBattleMonster.AddUObject(this, &UBossMonsterHPBarWidget::UpdateBossMonsterName);	
		}
	}
}
