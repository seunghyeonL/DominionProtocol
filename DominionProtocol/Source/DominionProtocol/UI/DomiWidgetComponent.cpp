// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiWidgetComponent.h"
#include "DomiUserWidget.h"

void UDomiWidgetComponent::InitWidget()
{
	Super::InitWidget();

	auto DomiCharacterWidget = Cast<UDomiUserWidget>(GetWidget());
	if (DomiCharacterWidget)
	{
		DomiCharacterWidget->SetOwningActor(GetOwner());
	}
}
