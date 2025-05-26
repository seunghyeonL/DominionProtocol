// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRunningEffect.h"

#include "Components/StatusComponent/StatusComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UPlayerRunningEffect::UPlayerRunningEffect()
{
	StatusEffectTag = EffectTags::Running;
	SpeedCoefficient = 1.5f;
	StaminaPerSecond = 5.0f;
}

void UPlayerRunningEffect::Activate()
{
	Super::Activate();
	
	if (!IsValid(OwnerCharacter))
	{
		Debug::PrintError(TEXT("UPlayerRunningEffect::Activate : Invalid OwnerCharacter."));
		return;
	}

	if (bIsActive)
	{
		return;
	}
	bIsActive = true;

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!StatusComponent)
	{
		Debug::PrintError(TEXT("UPlayerRunningEffect::Activate : Invalid StatusComponent."));
		return;
	}

	// StatusComponent->StopStaminaRecovery();
	auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
	// Debug::Print(TEXT("UPlayerRunningEffect::Activate : WalkSpeed Up."));
	MovementComponent->MaxWalkSpeed *= SpeedCoefficient;
}

void UPlayerRunningEffect::Activate(float Duration)
{
	// Super::Activate(Duration);
	Debug::PrintError(TEXT("UPlayerRunningEffect::Activate : Use Activate()!"));
}

void UPlayerRunningEffect::Deactivate()
{
	Super::Deactivate();
	
	if (!IsValid(OwnerCharacter))
	{
		Debug::PrintError(TEXT("UPlayerRunningEffect::Deactivate : Invalid OwnerCharacter."));
		return;
	}

	if (!bIsActive)
	{
		return;
	}
	bIsActive = false;

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	if (!StatusComponent)
	{
		Debug::PrintError(TEXT("UPlayerRunningEffect::Deactivate : Invalid StatusComponent."));
		return;
	}

	// StatusComponent->BlockStaminaRecovery();
	auto MovementComponent = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
	// Debug::Print(FString::Printf(TEXT("UPlayerRunningEffect::Deactivate : bIsActive : %d"), bIsActive ? 1 : 0));
	MovementComponent->MaxWalkSpeed /= SpeedCoefficient;
}

void UPlayerRunningEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto StatusComponent = Cast<UStatusComponent>(GetOuter());
	check(StatusComponent);

	if (FMath::IsNearlyZero(StatusComponent->GetStat(StatTags::Stamina)))
	{
		Deactivate();
	}
	
	StatusComponent->SetStamina(StatusComponent->GetStat(StatTags::Stamina) - StaminaPerSecond * DeltaTime);
}
