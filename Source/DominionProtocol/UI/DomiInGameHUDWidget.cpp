// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiInGameHUDWidget.h"

void UDomiInGameHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(StatusBar);
	ensure(AbilitySlots);
	ensure(ItemSlots);
	
}
