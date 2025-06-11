// Fill out your copyright notice in the Description page of Project Settings.


#include "StatModifyWidget.h"

#include "Components/Button.h"
#include "Util/GameTagList.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/StatusComponent/StatusComponent.h"

void UStatModifyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Collapsed);
	AActor* OwningActor = GetOwningPlayerPawn();

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

	TextBlockMap.Add(StatTags::STR, StrTextBlock);
	TextBlockMap.Add(StatTags::LIFE, LifeTextBlock);
	TextBlockMap.Add(StatTags::SPL, SplTextBlock);
	TextBlockMap.Add(StatTags::END, EndTextBlock);
	TextBlockMap.Add(StatTags::AttackPower, PrimaryAttackPowerTextBlock);
	TextBlockMap.Add(StatTags::SubAttackPower, SubAttackPowerTextBlock);
	TextBlockMap.Add(StatTags::MagicPower, MagicPowerTextBlock);
	TextBlockMap.Add(StatTags::MaxHealth, MaxHealthTextBlock);
	TextBlockMap.Add(StatTags::MaxStamina, MaxStaminaTextBlock);

	for (auto PlayerStatTag : PlayerStatTags)
	{
		PlayerStatPreviewData.Add(PlayerStatTag, 0.f);
	}
	
	auto* StatusComp = OwningActor->GetComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		StatusComp->GetPlayerStatData(PlayerStatPreviewData);
	}

	UpdatePreviewStat();
}

void UStatModifyWidget::UpdatePreviewStat()
{
	AActor* OwningActor = GetOwningPlayerPawn();
	auto* StatusComp = OwningActor->GetComponentByClass<UStatusComponent>();

	// 스탯 프리뷰 데이터 갱신
	StatusComp->UpdateStatPreviewData(PlayerStatPreviewData);

	// 현재 스탯 받아오기
	TMap<FGameplayTag, float> CurrentStatData;

	for (auto PlayerStatTag : PlayerStatTags)
	{
		CurrentStatData.Add(PlayerStatTag, 0.f);
	}

	
	StatusComp->GetPlayerStatData(CurrentStatData);

	// 바꿔진 스탯은 색깔 변경
	for (auto PlayerStatTag : PlayerStatTags)
	{
		if (PlayerStatPreviewData[PlayerStatTag] > CurrentStatData[PlayerStatTag])
		{
			Debug::Print(FString::Printf(TEXT("Preview, Real : %f, %f"), PlayerStatPreviewData[PlayerStatTag], CurrentStatData[PlayerStatTag]));
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
	AActor* OwningActor = GetOwningPlayerPawn();
	auto* StatusComp = OwningActor->GetComponentByClass<UStatusComponent>();
	StatusComp->DecideStatChangeFromUI(PlayerStatPreviewData);
	UpdatePreviewStat();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UStatModifyWidget::OnStrUpButtonClicked()
{
	PlayerStatPreviewData[StatTags::STR]++;
	UpdatePreviewStat();
}

void UStatModifyWidget::OnStrDownButtonClicked()
{
	PlayerStatPreviewData[StatTags::STR]--;
	UpdatePreviewStat();
}

void UStatModifyWidget::OnLifeUpButtonClicked()
{
	PlayerStatPreviewData[StatTags::LIFE]++;
	UpdatePreviewStat();
}

void UStatModifyWidget::OnLifeDownButtonClicked()
{
	PlayerStatPreviewData[StatTags::LIFE]--;
	UpdatePreviewStat();
}

void UStatModifyWidget::OnSplUpButtonClicked()
{
	PlayerStatPreviewData[StatTags::SPL]++;
	UpdatePreviewStat();
}

void UStatModifyWidget::OnSplDownButtonClicked()
{
	PlayerStatPreviewData[StatTags::SPL]--;
	UpdatePreviewStat();
}

void UStatModifyWidget::OnEndUpButtonClicked()
{
	PlayerStatPreviewData[StatTags::END]++;
	UpdatePreviewStat();
}

void UStatModifyWidget::OnEndDownButtonClicked()
{
	PlayerStatPreviewData[StatTags::END]--;
	UpdatePreviewStat();
}




