// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiMonster3DWidget.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "Components/WidgetComponent/DomiWidgetComponent.h"

void UDomiMonster3DWidget::UpdateMonsterHPBar(const float NewHP)
{
	AlphaForAnimMonsterHP = 0.f;
	PreMonsterHP = CurrentMonsterHP;
	CurrentMonsterHP = NewHP;
}

void UDomiMonster3DWidget::UpdateMonsterShield(const float NewShield)
{
	AlphaForAnimMonsterShield = 0.f;
	PreMonsterShield = CurrentMonsterShield;
	CurrentMonsterShield = NewShield;
}

void UDomiMonster3DWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	SetWidgetRotation();
}

void UDomiMonster3DWidget::SetOwningActor(AActor* NewOwner)
{
	Super::SetOwningActor(NewOwner);
	
	// Initialize & Binding
	SetupMonster3dWidget();
}

void UDomiMonster3DWidget::SetupMonster3dWidget()
{
	check(IsValid(OwningActor));

	TArray<UDomiWidgetComponent*> WidgetComponents;
	OwningActor->GetComponents<UDomiWidgetComponent>(WidgetComponents);

	for (auto* DomiWidgetComponent : WidgetComponents)
	{
		if (DomiWidgetComponent->GetName().Contains(TEXT("HPBarWidgetComponent")))
		{
			WidgetComponent = DomiWidgetComponent;
			break;
		}
	}
	
	auto* StatusComp = OwningActor->FindComponentByClass<UStatusComponent>();
	if (StatusComp)
	{
		// Initialize
		CurrentMonsterHP = StatusComp->GetStat(StatTags::Health);
		CurrentMonsterShield = StatusComp->GetStat(StatTags::Shield);
		MaxMonsterHP = StatusComp->GetStat(StatTags::MaxHealth);
		MaxMonsterShield = StatusComp->GetStat(StatTags::MaxShield);
		
		// DelegateBinding
		StatusComp->OnHealthChanged.AddDynamic(this, &UDomiMonster3DWidget::UpdateMonsterHPBar);
		StatusComp->OnShieldChanged.AddDynamic(this, &UDomiMonster3DWidget::UpdateMonsterShield);
	}
}

void UDomiMonster3DWidget::SetWidgetRotation() const
{
	check(IsValid(WidgetComponent));
	
	const auto* PlayerController = GetWorld()->GetFirstPlayerController();
	const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation(); 
	const FVector WidgetLocation = OwningActor->GetActorLocation();
	const FRotator LookAtRotation = FRotationMatrix::MakeFromX(CameraLocation - WidgetLocation).Rotator();
	
	WidgetComponent->SetWorldRotation(LookAtRotation);
}
