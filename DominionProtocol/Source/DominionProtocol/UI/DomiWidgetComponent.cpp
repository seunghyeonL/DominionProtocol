// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiWidgetComponent.h"
#include "DomiCharacterWidget.h"

void UDomiWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// SetOwner
	auto DomiCharacterWidget = Cast<UDomiCharacterWidget>(GetWidget());
	if (DomiCharacterWidget)
	{
		DomiCharacterWidget->SetOwningActor(GetOwner());
	}
}
