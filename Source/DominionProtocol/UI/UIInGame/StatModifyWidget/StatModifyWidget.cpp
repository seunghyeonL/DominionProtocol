// Fill out your copyright notice in the Description page of Project Settings.


#include "StatModifyWidget.h"

#include "Components/Button.h"
#include "Util/GameTagList.h"
#include "Components/TextBlock.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"

void UStatModifyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AActor* OwningActor = GetOwningPlayerPawn();
	PlayerStatusComponent = OwningActor->GetComponentByClass<UStatusComponent>();
	check(PlayerStatusComponent);

	PlayerStatTags.Add(StatTags::STR);
	PlayerStatTags.Add(StatTags::LIFE);
	PlayerStatTags.Add(StatTags::SPL);
	PlayerStatTags.Add(StatTags::END);
	PlayerStatTags.Add(StatTags::AttackPower);
	PlayerStatTags.Add(StatTags::SubAttackPower);
	PlayerStatTags.Add(StatTags::MagicPower);
	PlayerStatTags.Add(StatTags::MaxHealth);
	PlayerStatTags.Add(StatTags::MaxStamina);
	PlayerStatTags.Add(StatTags::Level);

	ActivatedStatModifyMap.Add(StatTags::STR, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::LIFE, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::SPL, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::END, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::AttackPower, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::SubAttackPower, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::MagicPower, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::MaxHealth, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::MaxStamina, FSlateColor(FLinearColor::White));
	ActivatedStatModifyMap.Add(StatTags::Level, FSlateColor(FLinearColor::White));

	StatModifiedNumMap.Add(StatTags::STR, 0);
	StatModifiedNumMap.Add(StatTags::LIFE, 0);
	StatModifiedNumMap.Add(StatTags::SPL, 0);
	StatModifiedNumMap.Add(StatTags::END, 0);
	StatModifiedNumMap.Add(StatTags::AttackPower, 0);
	StatModifiedNumMap.Add(StatTags::SubAttackPower, 0);
	StatModifiedNumMap.Add(StatTags::MagicPower, 0);
	StatModifiedNumMap.Add(StatTags::MaxHealth, 0);
	StatModifiedNumMap.Add(StatTags::MaxStamina, 0);
	StatModifiedNumMap.Add(StatTags::Level, 0);
	
	StatModifiedStringMap.Add(StatTags::STR, TEXT(""));
	StatModifiedStringMap.Add(StatTags::LIFE, TEXT(""));
	StatModifiedStringMap.Add(StatTags::SPL, TEXT(""));
	StatModifiedStringMap.Add(StatTags::END, TEXT(""));
	StatModifiedStringMap.Add(StatTags::AttackPower, TEXT(""));
	StatModifiedStringMap.Add(StatTags::SubAttackPower, TEXT(""));
	StatModifiedStringMap.Add(StatTags::MagicPower, TEXT(""));
	StatModifiedStringMap.Add(StatTags::MaxHealth, TEXT(""));
	StatModifiedStringMap.Add(StatTags::MaxStamina, TEXT(""));
	StatModifiedStringMap.Add(StatTags::Level, TEXT(""));

	for (auto PlayerStatTag : PlayerStatTags)
	{
		PlayerStatPreviewData.Add(PlayerStatTag, 0.f);
	}
	
	SetVisibility(ESlateVisibility::Collapsed);

	InitializeWidgetDatas();

	UpdatePreviewStat();
	
}

void UStatModifyWidget::InitializeWidgetDatas()
{
	AccumulatedRequiredEssence = 0.f;
	bCanLevelUp = false;
	
	for (auto PlayerStatTag : PlayerStatTags)
	{
		StatModifiedNumMap[PlayerStatTag] = 0.f;
	}
	
	PlayerStatusComponent->GetPlayerStatData(PlayerStatPreviewData);

	
	for (auto PlayerStatTag : PlayerStatTags)
	{
		StatModifiedStringMap[PlayerStatTag] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[PlayerStatTag]));
	}
}

void UStatModifyWidget::UpdatePreviewStat()
{
	// 레벨업 필요 재화
	auto DomiGI = Cast<UDomiGameInstance>(GetGameInstance());
	check(DomiGI);

	CurrentEssenceValue = DomiGI->GetPlayerCurrentEssence() - AccumulatedRequiredEssence;
	LevelUpRequiredEssence = PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	
	if (DomiGI->HasEnoughEssence(AccumulatedRequiredEssence + PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level])))
	{
		ActivatedLevelUpRequiredTextColor = FSlateColor(FLinearColor::Green);
		bCanLevelUp = true;
	}
	else
	{
		ActivatedLevelUpRequiredTextColor = FSlateColor(FLinearColor::Red);
		bCanLevelUp = false;
	}

	// 스탯 프리뷰 데이터 갱신
	PlayerStatusComponent->UpdateStatPreviewData(PlayerStatPreviewData);

	// 현재 스탯 받아오기
	TMap<FGameplayTag, float> CurrentStatData;

	for (auto PlayerStatTag : PlayerStatTags)
	{
		CurrentStatData.Add(PlayerStatTag, 0.f);
	}
	
	PlayerStatusComponent->GetPlayerStatData(CurrentStatData);

	// 바꿔진 스탯은 색깔 변경
	for (auto PlayerStatTag : PlayerStatTags)
	{
		if (PlayerStatPreviewData[PlayerStatTag] > CurrentStatData[PlayerStatTag])
		{
			ActivatedStatModifyMap[PlayerStatTag] = FSlateColor(FLinearColor::Green);
		}
		else
		{
			ActivatedStatModifyMap[PlayerStatTag] = FSlateColor(FLinearColor::White);
		}
	}

	UpdateStatModifyData();
}

void UStatModifyWidget::OnDecideButtonClicked()
{
	auto DomiGI = Cast<UDomiGameInstance>(GetGameInstance());
	check(DomiGI);
	
	DomiGI->SubtractPlayerCurrentEssence(AccumulatedRequiredEssence);
	PlayerStatusComponent->DecideStatChangeFromUI(PlayerStatPreviewData);

}

void UStatModifyWidget::OnStrUpButtonClicked()
{
	if (!bCanLevelUp) return;

	PlayerStatPreviewData[StatTags::STR]++;
	StatModifiedNumMap[StatTags::STR]++;
	StatModifiedStringMap[StatTags::STR] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::STR]), StatModifiedNumMap[StatTags::STR]);
	StatModifiedStringMap[StatTags::AttackPower] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::AttackPower]), StatModifiedNumMap[StatTags::AttackPower]);
	StatModifiedStringMap[StatTags::SubAttackPower] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::SubAttackPower]), StatModifiedNumMap[StatTags::SubAttackPower]);
	
	
	AccumulatedRequiredEssence += PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]++;
	StatModifiedNumMap[StatTags::Level]++;
	StatModifiedStringMap[StatTags::Level] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::Level]), StatModifiedNumMap[StatTags::Level]);
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnStrDownButtonClicked()
{
	if (StatModifiedNumMap[StatTags::STR] == 0) return;
	
	PlayerStatPreviewData[StatTags::STR]--;
	StatModifiedNumMap[StatTags::STR]--;
	if (StatModifiedNumMap[StatTags::STR] == 0)
	{
		StatModifiedStringMap[StatTags::STR] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::STR]));
		StatModifiedStringMap[StatTags::AttackPower] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::AttackPower]));
		StatModifiedStringMap[StatTags::SubAttackPower] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::SubAttackPower]));
	
	}
	else
	{
		StatModifiedStringMap[StatTags::STR] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::STR]), StatModifiedNumMap[StatTags::STR]);
		StatModifiedStringMap[StatTags::AttackPower] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::AttackPower]), StatModifiedNumMap[StatTags::AttackPower]);
		StatModifiedStringMap[StatTags::SubAttackPower] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::SubAttackPower]), StatModifiedNumMap[StatTags::SubAttackPower]);
	
	}
	
	
	AccumulatedRequiredEssence -= PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]--;
	StatModifiedNumMap[StatTags::Level]--;
	StatModifiedStringMap[StatTags::Level] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::Level]), StatModifiedNumMap[StatTags::Level]);
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnLifeUpButtonClicked()
{
	if (!bCanLevelUp) return;
	PlayerStatPreviewData[StatTags::LIFE]++;
	StatModifiedNumMap[StatTags::LIFE]++;
	StatModifiedStringMap[StatTags::LIFE] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::LIFE]), StatModifiedNumMap[StatTags::LIFE]);
	StatModifiedStringMap[StatTags::MaxHealth] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MaxHealth]), StatModifiedNumMap[StatTags::MaxHealth]);
	
	
	AccumulatedRequiredEssence += PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]++;
	StatModifiedNumMap[StatTags::Level]++;
	StatModifiedStringMap[StatTags::Level] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::Level]), StatModifiedNumMap[StatTags::Level]);
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnLifeDownButtonClicked()
{
	if (StatModifiedNumMap[StatTags::LIFE] == 0) return;
	
	PlayerStatPreviewData[StatTags::LIFE]--;
	StatModifiedNumMap[StatTags::LIFE]--;
	if (StatModifiedNumMap[StatTags::LIFE] == 0)
	{
		StatModifiedStringMap[StatTags::LIFE] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::LIFE]));
		StatModifiedStringMap[StatTags::MaxHealth] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MaxHealth]));
	}
	else
	{
		StatModifiedStringMap[StatTags::LIFE] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::LIFE]), StatModifiedNumMap[StatTags::LIFE]);
		StatModifiedStringMap[StatTags::MaxHealth] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MaxHealth]), StatModifiedNumMap[StatTags::MaxHealth]);
	}
	
	AccumulatedRequiredEssence -= PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]--;
	StatModifiedNumMap[StatTags::Level]--;
	StatModifiedStringMap[StatTags::Level] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::Level]), StatModifiedNumMap[StatTags::Level]);
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnSplUpButtonClicked()
{
	if (!bCanLevelUp) return;
	
	PlayerStatPreviewData[StatTags::SPL]++;
	StatModifiedNumMap[StatTags::SPL]++;
	StatModifiedStringMap[StatTags::SPL] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::SPL]), StatModifiedNumMap[StatTags::SPL]);
	StatModifiedStringMap[StatTags::MagicPower] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MagicPower]), StatModifiedNumMap[StatTags::MagicPower]);
	
	AccumulatedRequiredEssence += PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]++;
	StatModifiedNumMap[StatTags::Level]++;
	StatModifiedStringMap[StatTags::Level] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::Level]), StatModifiedNumMap[StatTags::Level]);
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnSplDownButtonClicked()
{
	if (StatModifiedNumMap[StatTags::SPL] == 0) return;
	
	PlayerStatPreviewData[StatTags::SPL]--;
	StatModifiedNumMap[StatTags::SPL]--;
	if (StatModifiedNumMap[StatTags::SPL] == 0)
	{
		StatModifiedStringMap[StatTags::SPL] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::SPL]));
		StatModifiedStringMap[StatTags::MagicPower] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MagicPower]));
	}
	else
	{
		StatModifiedStringMap[StatTags::SPL] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::SPL]), StatModifiedNumMap[StatTags::SPL]);
		StatModifiedStringMap[StatTags::MagicPower] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MagicPower]), StatModifiedNumMap[StatTags::MagicPower]);
	}
	
	AccumulatedRequiredEssence -= PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]--;
	StatModifiedNumMap[StatTags::Level]--;
	StatModifiedStringMap[StatTags::Level] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::Level]), StatModifiedNumMap[StatTags::Level]);
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnEndUpButtonClicked()
{
	if (!bCanLevelUp) return;

	PlayerStatPreviewData[StatTags::END]++;
	StatModifiedNumMap[StatTags::END]++;
	StatModifiedStringMap[StatTags::END] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::END]), StatModifiedNumMap[StatTags::END]);
	StatModifiedStringMap[StatTags::MaxStamina] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MaxStamina]), StatModifiedNumMap[StatTags::MaxStamina]);
	
	AccumulatedRequiredEssence += PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]++;
	StatModifiedNumMap[StatTags::Level]++;
	StatModifiedStringMap[StatTags::Level] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::Level]), StatModifiedNumMap[StatTags::Level]);
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnEndDownButtonClicked()
{
	if (StatModifiedNumMap[StatTags::END] == 0) return;
	
	PlayerStatPreviewData[StatTags::END]--;
	StatModifiedNumMap[StatTags::END]--;
	if (StatModifiedNumMap[StatTags::END] == 0)
	{
		StatModifiedStringMap[StatTags::END] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::END]));
		StatModifiedStringMap[StatTags::MaxStamina] = FString::Printf(TEXT("%d"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MaxStamina]));
	}
	else
	{
		StatModifiedStringMap[StatTags::END] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::END]), StatModifiedNumMap[StatTags::END]);
		StatModifiedStringMap[StatTags::MaxStamina] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::MaxStamina]), StatModifiedNumMap[StatTags::MaxStamina]);
	}
	
	AccumulatedRequiredEssence -= PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]--;
	StatModifiedNumMap[StatTags::Level]--;
	StatModifiedStringMap[StatTags::Level] = FString::Printf(TEXT("%d (+%d)"), FMath::FloorToInt(PlayerStatPreviewData[StatTags::Level]), StatModifiedNumMap[StatTags::Level]);
	
	UpdatePreviewStat();
}




