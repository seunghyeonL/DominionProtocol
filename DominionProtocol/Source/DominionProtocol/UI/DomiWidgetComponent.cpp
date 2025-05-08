// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiWidgetComponent.h"
#include "DomiCharacter3DWidget.h"

void UDomiWidgetComponent::InitWidget()
{
	Super::InitWidget();

	// SetOwner
	auto DomiCharacterWidget = Cast<UDomiCharacter3DWidget>(GetWidget());
	if (DomiCharacterWidget)
	{
		DomiCharacterWidget->SetOwningActor(GetOwner());
	}
}
