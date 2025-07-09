// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BasePlayerController.h"

#include "UI/FadeInOut/FadeWidget.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"

ABasePlayerController::ABasePlayerController()
{
	FadeDuration = 1.f;
}

void ABasePlayerController::FadeIn(const float PlayTime)
{
	if (FadeWidgetInstance)
	{
		// 파라미터 입력이 없을 경우
		if (PlayTime < 0.f)
		{
			FadeWidgetInstance->FadeIn(FadeDuration);
		}
		else
		{
			FadeWidgetInstance->FadeIn(PlayTime);
		}
	}
}

void ABasePlayerController::FadeOut(const float PlayTime)
{
	if (FadeWidgetInstance)
	{
		// 파라미터 입력이 없을 경우
		if (PlayTime < 0.f)
		{
			FadeWidgetInstance->FadeOut(FadeDuration);	
		}
		else
		{
			FadeWidgetInstance->FadeOut(PlayTime);	
		}
	}
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (LocalPlayer)
	{
		LocalPlayerInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	}

	CreateAndAddHUDWidget();
	SetupInputMode();
	SetupMappingContext();
	BindInputActions();
}

void ABasePlayerController::CreateAndAddHUDWidget()
{
	check(IsValid(FadeWidgetClass));
	
	FadeWidgetInstance = CreateWidget<UFadeWidget>(this, FadeWidgetClass);

	if (FadeWidgetInstance)
	{
		FadeWidgetInstance->AddToViewport();
	}
}

void ABasePlayerController::SetupInputMode()
{
}

void ABasePlayerController::SetupMappingContext()
{
}

void ABasePlayerController::BindInputActions()
{
}

void ABasePlayerController::RemoveAllMappingContext()
{
	for (const UInputMappingContext* MappingContext : MappingContextArray)
	{
		LocalPlayerInputSubsystem->RemoveMappingContext(MappingContext);
	}
}

void ABasePlayerController::HelperBindInputAction(UEnhancedInputComponent* EnhancedInputComponent, const UInputAction* Action, const ETriggerEvent Event, const FName FunctionName)
{
	check(EnhancedInputComponent);
	check(Action);

	if (!EnhancedInputComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ABasePlayerController::HelperBindInputAction - EnhancedInputComponent is null"))
		return;
	}

	if (IsValid(Action))
	{
		EnhancedInputComponent->BindAction(Action, Event, this, FunctionName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ABasePlayerController::HelperBindInputAction - %s Action is null"), *FunctionName.ToString());
	}
}
