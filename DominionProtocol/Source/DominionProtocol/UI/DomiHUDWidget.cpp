// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiHUDWidget.h"

void UDomiHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(StatusBar);
	ensure(AbilitySlots);
	ensure(ItemSlots);
	
}
