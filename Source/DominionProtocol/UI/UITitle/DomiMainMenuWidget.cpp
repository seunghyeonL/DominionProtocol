// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiMainMenuWidget.h"

void UDomiMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(StartGameButton);
	ensure(OptionButton);
	ensure(ExitButton);
}
