// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiInteractionContentWidget.h"

#include "Player/Characters/DomiCharacter.h"

void UDomiInteractionContentWidget::SetReceivedInteractableActor(AActor* Actor = nullptr)
{
	ReceivedInteractableActor = Actor;
	OwningCharacter->SetCurrentInteractableActor(ReceivedInteractableActor);
	bHasFocus = true;
}

void UDomiInteractionContentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwningCharacter = Cast<ADomiCharacter>(GetOwningPlayerPawn());
}
