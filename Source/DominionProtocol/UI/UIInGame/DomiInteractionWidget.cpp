// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiInteractionWidget.h"

#include "Components/WidgetComponent/DomiWidgetComponent.h"
#include "Player/Characters/DomiCharacter.h"

void UDomiInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindInteractionDelegates();
}

void UDomiInteractionWidget::UpdateInteractableActorArray(TArray<AActor*> NewInteractableActorArray)
{
	InteractableActorArray = NewInteractableActorArray;
	UpdateInteractionWidget();
}

void UDomiInteractionWidget::BindInteractionDelegates()
{
	auto* PlayerCharacter = Cast<ADomiCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnAddInteractableActor.AddUObject(this, &UDomiInteractionWidget::UpdateInteractableActorArray);
		PlayerCharacter->OnRemoveInteractableActor.AddUObject(this, &UDomiInteractionWidget::UpdateInteractableActorArray);
	}
}


