// Fill out your copyright notice in the Description page of Project Settings.


#include "DomiStatusBarWidget.h"

void UDomiStatusBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Check Widget Binding 
	ensure(PlayerHPBar);
	ensure(PlayerStaminaBar);

	// Initialize & DelegateBinding
	SetupHUDWidget();
}

void UDomiStatusBarWidget::SetupHUDWidget()
{
	
}
