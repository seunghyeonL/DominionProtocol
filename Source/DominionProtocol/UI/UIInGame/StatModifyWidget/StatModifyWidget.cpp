// Fill out your copyright notice in the Description page of Project Settings.


#include "StatModifyWidget.h"

#include "Components/Button.h"
#include "Util/GameTagList.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"

void UStatModifyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AActor* OwningActor = GetOwningPlayerPawn();
	PlayerStatusComponent = OwningActor->GetComponentByClass<UStatusComponent>();
	check(PlayerStatusComponent);

	StrUpButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnStrUpButtonClicked);
	StrDownButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnStrDownButtonClicked);
	LifeUpButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnLifeUpButtonClicked);
	LifeDownButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnLifeDownButtonClicked);
	SplUpButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnSplUpButtonClicked);
	SplDownButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnSplDownButtonClicked);
	EndUpButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnEndUpButtonClicked);
	EndDownButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnEndDownButtonClicked);
	DecideButton->OnClicked.AddDynamic(this, &UStatModifyWidget::OnDecideButtonClicked);

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

	TextBlockMap.Add(StatTags::STR, StrTextBlock);
	TextBlockMap.Add(StatTags::LIFE, LifeTextBlock);
	TextBlockMap.Add(StatTags::SPL, SplTextBlock);
	TextBlockMap.Add(StatTags::END, EndTextBlock);
	TextBlockMap.Add(StatTags::AttackPower, PrimaryAttackPowerTextBlock);
	TextBlockMap.Add(StatTags::SubAttackPower, SubAttackPowerTextBlock);
	TextBlockMap.Add(StatTags::MagicPower, MagicPowerTextBlock);
	TextBlockMap.Add(StatTags::MaxHealth, MaxHealthTextBlock);
	TextBlockMap.Add(StatTags::MaxStamina, MaxStaminaTextBlock);
	TextBlockMap.Add(StatTags::Level, LevelTextBlock);

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
}

void UStatModifyWidget::UpdatePreviewStat()
{
	// 레벨업 필요 재화
	auto DomiGI = Cast<UDomiGameInstance>(GetGameInstance());
	check(DomiGI);

	CurrentEssenceTextBlock->SetText(FText::AsNumber(DomiGI->GetPlayerCurrentEssence() - AccumulatedRequiredEssence));
	LevelUpRequiredTextBlock->SetText(FText::AsNumber(PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level])));
	AccumulatedEssenceTextBlock->SetText(FText::AsNumber(AccumulatedRequiredEssence));
	
	if (DomiGI->HasEnoughEssence(AccumulatedRequiredEssence + PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level])))
	{
		LevelUpRequiredTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		bCanLevelUp = true;
	}
	else {
		LevelUpRequiredTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
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
			TextBlockMap[PlayerStatTag]->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
		}
		else
		{
			TextBlockMap[PlayerStatTag]->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		}
	}

	// 프리뷰 스탯 수치 텍스트블록에 보여주기
	for (auto PlayerStatTag : PlayerStatTags)
	{
		TextBlockMap[PlayerStatTag]->SetText(FText::AsNumber(PlayerStatPreviewData[PlayerStatTag]));
	}
}

void UStatModifyWidget::OnDecideButtonClicked()
{
	auto DomiGI = Cast<UDomiGameInstance>(GetGameInstance());
	check(DomiGI);
	
	DomiGI->SubtractPlayerCurrentEssence(AccumulatedRequiredEssence);
	PlayerStatusComponent->DecideStatChangeFromUI(PlayerStatPreviewData);
	
	SetVisibility(ESlateVisibility::Collapsed);
	InitializeWidgetDatas();
	UpdatePreviewStat();
}

void UStatModifyWidget::OnStrUpButtonClicked()
{
	if (!bCanLevelUp) return;

	PlayerStatPreviewData[StatTags::STR]++;
	StatModifiedNumMap[StatTags::STR]++;
	
	AccumulatedRequiredEssence += PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]++;
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnStrDownButtonClicked()
{
	if (StatModifiedNumMap[StatTags::STR] == 0) return;
	
	PlayerStatPreviewData[StatTags::STR]--;
	StatModifiedNumMap[StatTags::STR]--;
	
	AccumulatedRequiredEssence -= PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]--;
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnLifeUpButtonClicked()
{
	if (!bCanLevelUp) return;
	PlayerStatPreviewData[StatTags::LIFE]++;
	StatModifiedNumMap[StatTags::LIFE]++;
	
	AccumulatedRequiredEssence += PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]++;
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnLifeDownButtonClicked()
{
	if (StatModifiedNumMap[StatTags::LIFE] == 0) return;
	
	PlayerStatPreviewData[StatTags::LIFE]--;
	StatModifiedNumMap[StatTags::LIFE]--;
	
	AccumulatedRequiredEssence -= PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]--;
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnSplUpButtonClicked()
{
	if (!bCanLevelUp) return;
	
	PlayerStatPreviewData[StatTags::SPL]++;
	StatModifiedNumMap[StatTags::SPL]++;
	
	AccumulatedRequiredEssence += PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]++;
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnSplDownButtonClicked()
{
	if (StatModifiedNumMap[StatTags::SPL] == 0) return;
	
	PlayerStatPreviewData[StatTags::SPL]--;
	StatModifiedNumMap[StatTags::SPL]--;
	
	AccumulatedRequiredEssence -= PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]--;
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnEndUpButtonClicked()
{
	if (!bCanLevelUp) return;

	PlayerStatPreviewData[StatTags::END]++;
	StatModifiedNumMap[StatTags::END]++;
	
	AccumulatedRequiredEssence += PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]++;
	
	UpdatePreviewStat();
}

void UStatModifyWidget::OnEndDownButtonClicked()
{
	if (StatModifiedNumMap[StatTags::END] == 0) return;
	
	PlayerStatPreviewData[StatTags::END]--;
	StatModifiedNumMap[StatTags::END]--;
	
	AccumulatedRequiredEssence -= PlayerStatusComponent->GetLevelUpRequiredEssence(PlayerStatPreviewData[StatTags::Level]);
	PlayerStatPreviewData[StatTags::Level]--;
	
	UpdatePreviewStat();
}




