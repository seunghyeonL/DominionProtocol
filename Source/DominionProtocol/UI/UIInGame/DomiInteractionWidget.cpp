// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiInteractionWidget.h"

#include "Components/WidgetComponent/DomiWidgetComponent.h"
#include "Player/Characters/DomiCharacter.h"

void UDomiInteractionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindInteractionDelegates();

	OwningCharacter = Cast<ADomiCharacter>(GetOwningPlayerPawn());
}

void UDomiInteractionWidget::UpdateInteractableActorSet(TSet<AActor*> NewInteractableActorSet)
{
	InteractableActorSet = NewInteractableActorSet;
	UpdateInteractionWidget();
}

void UDomiInteractionWidget::UpdateInteractableWidgetFocusing(const float ScrollValue)
{
	ChangeWidgetFocus(ScrollValue);
}

void UDomiInteractionWidget::BindInteractionDelegates()
{
	auto* PlayerCharacter = Cast<ADomiCharacter>(GetOwningPlayerPawn());
	if (PlayerCharacter)
	{
		PlayerCharacter->OnAddInteractableActor.AddUObject(this, &UDomiInteractionWidget::UpdateInteractableActorSet);
		PlayerCharacter->OnRemoveInteractableActor.AddUObject(this, &UDomiInteractionWidget::UpdateInteractableActorSet);
		PlayerCharacter->OnInteractionWidgetScroll.AddUObject(this, &UDomiInteractionWidget::UpdateInteractableWidgetFocusing);
	}
}

void UDomiInteractionWidget::SetInteractableActorEmpty()
{
	ensure(OwningCharacter);

	OwningCharacter->SetCurrentInteractableActor(nullptr);
}


