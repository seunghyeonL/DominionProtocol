// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RenewalUI/NewEquipmentWidget.h"

#include "Components/ItemComponent/ItemComponent.h"
#include "Components/StatusComponent/StatusComponent.h"

void UNewEquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto* PlayerStatusComponent = GetOwningPlayerPawn()->FindComponentByClass(UStatusComponent::StaticClass());
	if (PlayerStatusComponent)
	{
		StatusComponent = Cast<UStatusComponent>(PlayerStatusComponent);
	}

	auto* PlayerItemComponent = GetOwningPlayerPawn()->FindComponentByClass(UItemComponent::StaticClass());
	if (PlayerItemComponent)
	{
		ItemComponent = Cast<UItemComponent>(PlayerItemComponent);
	}
}
