// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/NewTargetLockOnHUDWidget.h"

#include "Components/PlayerControlComponent/PlayerControlComponent.h"

void UNewTargetLockOnHUDWidget::UpdateLockOnTargetActor(AActor* NewTarget)
{
	CurrentTargetActor = NewTarget;
}

void UNewTargetLockOnHUDWidget::ChangeActivateLockOn(bool bIsActive)
{
	bIsActiveLockOn = bIsActive;

	PlaySearchingLockOnTargetAnim();
}

void UNewTargetLockOnHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BindLockOnTargetActorDelegate();
}


void UNewTargetLockOnHUDWidget::BindLockOnTargetActorDelegate()
{
	auto* PlayerControlComponent = GetOwningPlayerPawn()->GetComponentByClass<UPlayerControlComponent>();
	if (PlayerControlComponent)
	{
		// Is Current Target the Owning Actor?
		PlayerControlComponent->OnSetLockOnTarget.AddUObject(this, &UNewTargetLockOnHUDWidget::UpdateLockOnTargetActor);
		
		// Is LockOn Active?
		PlayerControlComponent->OnActiveLockOn.AddUObject(this, &UNewTargetLockOnHUDWidget::ChangeActivateLockOn);
	}
}
