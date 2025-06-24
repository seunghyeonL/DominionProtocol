// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/AlertWidget.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "WorldObjects/Crack.h"
#include "WorldObjects/DropEssence.h"

void UAlertWidget::OnShowPlayerDeathAlert()
{
	ShowPlayerDeathAlert();
}

void UAlertWidget::OnShowBossKillAlert(AActor* DeadMonster)
{
	ShowBossKillAlert(DeadMonster);
}

void UAlertWidget::OnShowEssenceRestoredAlert(const int32 NewEssenceAmount)
{
	ShowEssenceRestoredAlert(NewEssenceAmount);

	// 에센스 획득 후 델리게이트 핸들 삭제, 에센스 자체에 있는 델리게이트는 파괴시 삭제되지만 이 경우엔 명시적으로 삭제해주면 좋음
	auto* InGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (InGameMode)
	{
		InGameMode->OnSpawnDropEssence.Remove(HandleBindRestoredEssence);
	}
}

void UAlertWidget::OnShowCrackActivationAlert(const FText CrackName)
{
	ShowCrackActivationAlert(CrackName);
}

void UAlertWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindAlertDelegates();
}

void UAlertWidget::BindAlertDelegates()
{
	// 균열 활성화 바인딩 섹션
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACrack::StaticClass(), FoundActors);

	for (auto* Actor : FoundActors)
	{
		auto* Crack = Cast<ACrack>(Actor);
		if (Crack)
		{
			Crack->OnActiveCrack.AddUObject(this, &UAlertWidget::OnShowCrackActivationAlert);
		}
	}

	// 플레이어 데스 바인딩 섹션
	auto* StatusComponent = GetOwningPlayerPawn()->GetComponentByClass<UStatusComponent>();
	if (StatusComponent)
	{
		StatusComponent->OnDeath.AddUObject(this, &UAlertWidget::OnShowPlayerDeathAlert);
	}

	// 보스 사망 및 DropEssence 바인딩 섹션
	auto* InGameMode = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());
	if (InGameMode)
	{
		InGameMode->OnEndBattle.AddUObject(this, &UAlertWidget::OnShowBossKillAlert);
		HandleBindRestoredEssence = InGameMode->OnSpawnDropEssence.AddUObject(this, &UAlertWidget::BindRestoredEssence);
	}
}

void UAlertWidget::BindRestoredEssence(class ADropEssence* DropEssence)
{
	DropEssence->OnDropEssenceRestored.AddUObject(this, &UAlertWidget::OnShowEssenceRestoredAlert);
}