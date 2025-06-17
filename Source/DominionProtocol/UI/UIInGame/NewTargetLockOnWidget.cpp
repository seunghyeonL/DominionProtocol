// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIInGame/NewTargetLockOnWidget.h"

#include "Components/WidgetComponent.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/WidgetComponent/DomiWidgetComponent.h"

void UNewTargetLockOnWidget::UpdateLockOnTargetActor(AActor* NewTargetActor)
{
	CurrentTargetActor = NewTargetActor;
	
	OnUpdateTargetLockOnActor();
}

void UNewTargetLockOnWidget::ChangeActivateLockOn(bool bIsActive)
{
	bIsActiveLockOn = bIsActive;

	OnChangeActivateLockOn();
}


void UNewTargetLockOnWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	SetWidgetRotation();

	// Lock On 조건 찾아보고 없애도 되면 없애기
}

void UNewTargetLockOnWidget::SetWidgetRotation()
{
	check(IsValid(WidgetComponent));
	
	const auto* PlayerController = GetWorld()->GetFirstPlayerController();
	const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation(); 
	const FVector WidgetLocation = OwningActor->GetActorLocation();
	const FRotator LookAtRotation = FRotationMatrix::MakeFromX(CameraLocation - WidgetLocation).Rotator();
	
	WidgetComponent->SetWorldRotation(LookAtRotation);
}

void UNewTargetLockOnWidget::SetOwningActor(AActor* NewOwner)
{
	Super::SetOwningActor(NewOwner);

	// Trigger : Monster's WidgetComponent -> InitWidget()
	SetupTargetLockOnWidget();
}

void UNewTargetLockOnWidget::SetupTargetLockOnWidget()
{
	check(IsValid(OwningActor));

	// Delegate Binding Section
	auto* World = GetWorld();
	if (World)
	{
		auto* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			auto* PlayerPawn = PlayerController->GetPawn();
			if (PlayerPawn)
			{
				auto* PlayerControlComponent = PlayerPawn->GetComponentByClass<UPlayerControlComponent>();
				if (PlayerControlComponent)
				{
					// Is Current Target the Owning Actor?
					PlayerControlComponent->OnSetLockOnTarget.AddUObject(this, &UNewTargetLockOnWidget::UpdateLockOnTargetActor);
		
					// Is LockOn Active?
					PlayerControlComponent->OnActiveLockOn.AddUObject(this, &UNewTargetLockOnWidget::ChangeActivateLockOn);
				}	
			}
		}
	}

	// for SetWidgetRotation
	TArray<UDomiWidgetComponent*> WidgetComponents;
	OwningActor->GetComponents<UDomiWidgetComponent>(WidgetComponents);
	
	for (auto* DomiWidgetComponent : WidgetComponents)
	{
		if (DomiWidgetComponent->GetName().Contains(TEXT("LockOnWidgetComponent")))
		{
			WidgetComponent = DomiWidgetComponent;
		}
	}
}

