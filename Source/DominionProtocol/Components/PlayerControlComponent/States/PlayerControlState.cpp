// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlState.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interface/InteractableInterface.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "Kismet/GameplayStatics.h"

UPlayerControlState::UPlayerControlState()
{
	OwnerCharacter = nullptr;
	OuterState = nullptr;
}

void UPlayerControlState::Tick(float DeltaTime)
{
}

void UPlayerControlState::Move(const FInputActionValue& Value)
{
	Super::Move(Value);
	// input is a Vector2D
	FVector2D InputVector = Value.Get<FVector2D>();

	if (!OwnerCharacter)
	{
		Debug::PrintError(TEXT("UPlayerControlState::Move : Invalid OwnerCharacter."));
		return;
	}

	if (auto Controller = OwnerCharacter->GetController())
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// final movement vector
		const FVector FinalMovementNormalVector = (ForwardDirection * InputVector.X  + RightDirection * InputVector.Y).GetSafeNormal();

		if (auto ControlComponentUser = Cast<IControlComponentUser>(OwnerCharacter))
		{
			ControlComponentUser->SetLastMovementVector(FinalMovementNormalVector);
		}

		// add movement
		OwnerCharacter->AddMovementInput(FinalMovementNormalVector);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::Move : Invalid Controller."));
	}
}

void UPlayerControlState::Look(const FInputActionValue& Value)
{
	Super::Look(Value);

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (!OwnerCharacter)
	{
		Debug::PrintError(TEXT("UPlayerControlState::Look : Invalid OwnerCharacter."));
		return;
	}
	
	if (auto Controller = OwnerCharacter->GetController())
	{
		// add yaw and pitch input to controller
		OwnerCharacter->AddControllerYawInput(LookAxisVector.X);
		OwnerCharacter->AddControllerPitchInput(LookAxisVector.Y);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::Look : Invalid Controller."));
	}
}

void UPlayerControlState::Dash()
{
	Super::Dash();

	if (!OwnerCharacter) return;

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::Dash);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::BaseAttack : Character doesn't implement ISkillComponentUser."));
	}
	// if (ADomiCharacter* DomiChar = Cast<ADomiCharacter>(OwnerCharacter))
	// {
	// 	if (!DomiChar->StartDash()) return;
	//
	// 	DomiChar->DashElapsed = 0.f;
	// 	bDashTickActive = true;
	//
	// 	DomiChar->DashMoveDirection = DomiChar->GetDashDirection();
	// }
}

void UPlayerControlState::SprintStart()
{
	Super::SprintStart();

	auto StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();
	check(StatusComponent);

	StatusComponent->ActivateStatusEffect(EffectTags::Running, 0.f);
}

void UPlayerControlState::SprintEnd()
{
	Super::SprintEnd();
	
	auto StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();
	check(StatusComponent);

	StatusComponent->DeactivateStatusEffect(EffectTags::Running);
}

void UPlayerControlState::Parry()
{
	Super::Parry();

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::Parry);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::BaseAttack : Character doesn't implement ISkillComponentUser."));
	}
}

void UPlayerControlState::BaseAttack()
{
	Super::BaseAttack();

	// ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	check(OwnerCharacter);

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::BaseAttack);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::BaseAttack : Character doesn't implement ISkillComponentUser."));
	}

}

void UPlayerControlState::WeaponSkill()
{
	Super::WeaponSkill();

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::WeaponSkill);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::WeaponSkill : Character doesn't implement ISkillComponentUser."));
	}
}

void UPlayerControlState::MagicSkill()
{
	Super::MagicSkill();

	check(OwnerCharacter);

	if (auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerCharacter))
	{
		SkillComponentUser->ExecuteSkill(SkillGroupTags::MagicSkill);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::MasicSkill : Character doesn't implement ISkillComponentUser."));
	}
}

void UPlayerControlState::Interact()
{
	Super::Interact();

	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	check(PlayerCharacter);

	AActor* InteractableObject = PlayerCharacter->GetCurrentInteractableActor();
	if (IsValid(InteractableObject))
	{
		IInteractableInterface::Execute_Interact(InteractableObject, PlayerCharacter);
		FText UIText = IInteractableInterface::Execute_GetInteractMessage(InteractableObject);
		
	}
	
	Debug::Print(TEXT("UPlayerControlState::Interact : Call."));
}

void UPlayerControlState::LockOn()
{
	Super::LockOn();
	UPlayerControlComponent* PlayerControlComponent= Cast<UPlayerControlComponent>(GetOuter());
	if (PlayerControlComponent->GetActiveControlEffectTags().HasTag(EffectTags::LockOn))
	{
		PlayerControlComponent->DeactivateControlEffect(EffectTags::LockOn);
	}
	else
	{
		if (SetLockOnTargetActorInPublicSpace() == false)
		{
			SetLockOnTargetActorInVisibility();
		}

		// ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter);
		if (IsValid(PlayerControlComponent->GetLockOnTargetActor()))
		{
			PlayerControlComponent->ActivateControlEffect(EffectTags::LockOn);
		}
		else
		{
			Debug::PrintLog(TEXT("UPlayerControlState::LockOn : No LockOnTargetActor."));
		}
	}

	Debug::Print(TEXT("UPlayerControlState::LockOn : Call."));
}

bool UPlayerControlState::SetLockOnTargetActorInPublicSpace()
{
	const FVector Start = OwnerCharacter->GetActorLocation();
	const FVector End = Start;
	TArray<FHitResult> SphereTraceHitResults;
	FCollisionQueryParams SphereTraceQueryParams;
	SphereTraceQueryParams.AddIgnoredActor(OwnerCharacter);
	constexpr float PublicSpaceDistance = 760.f;
	
	bool bHit = GetWorld()->SweepMultiByChannel(
		SphereTraceHitResults,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(PublicSpaceDistance),
		SphereTraceQueryParams
	);

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	ControlComponent->SetLockOnTargetActor(nullptr);
	
	if (bHit)
	{
		float MinDistance = PublicSpaceDistance;
		AActor* MinDistanceActor = nullptr;
		for (const FHitResult& Hit : SphereTraceHitResults)
		{
			AActor* HitActor = Hit.GetActor();

			if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
			{
				if (IsActorInViewport(HitActor->GetActorLocation()))
				{
					const float ActorDistance = FVector::Distance(OwnerCharacter->GetActorLocation(), HitActor->GetActorLocation()); 
					if (ActorDistance < MinDistance)
					{
						MinDistance = ActorDistance;
						MinDistanceActor = HitActor;
					}
				}
			}
		}
		if (MinDistanceActor)
		{
			// Target actor selected.
			ControlComponent->SetLockOnTargetActor(MinDistanceActor);
			return true;
		}
	}
	// There is no target actor.
	return false;
}

bool UPlayerControlState::SetLockOnTargetActorInVisibility()
{
	TArray<FHitResult> SphereTraceHitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	constexpr float VisibilityDistance = 3000.f;
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerCharacter->GetController()->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	const FRotator ControllerRotatorWithZeroPitch = FRotator(0,ViewPointRotation.Yaw, ViewPointRotation.Roll);
	const FVector ControllerForwardVector = ControllerRotatorWithZeroPitch.Vector();
	
	const FVector Start = ViewPointLocation + FMath::Sqrt(2.f) * ControllerForwardVector * VisibilityDistance;
	const FVector End = Start;

	const FVector BoxSize = FVector(VisibilityDistance,VisibilityDistance,600.f);
	
	bool bBoxTraceHit = GetWorld()->SweepMultiByChannel(
		SphereTraceHitResults,
		Start,
		End,
		FQuat(FRotator(0,ViewPointRotation.Yaw + 45.f,0)),
		ECC_Visibility,
		FCollisionShape::MakeBox(BoxSize),
		QueryParams
	);

	auto ControlComponent = Cast<UPlayerControlComponent>(GetOuter());
	ControlComponent->SetLockOnTargetActor(nullptr);
	
	if (bBoxTraceHit)
	{
		float MinAngle = 90.f;
		AActor* MinAngleActor = nullptr;
		for (const FHitResult& Hit : SphereTraceHitResults)
		{
			AActor* HitActor = Hit.GetActor();

			if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
			{
				if (IsActorInViewport(HitActor->GetActorLocation()))
				{
					FHitResult LineTraceHit;
					bool bLineTraceHit = GetWorld()->LineTraceSingleByChannel(
						LineTraceHit,
						OwnerCharacter->GetActorLocation(),
						HitActor->GetActorLocation(),
						ECC_Visibility,
						QueryParams
					);
					if (bLineTraceHit && LineTraceHit.GetActor() == HitActor)
					{
						FVector ControllerToActorVector = (HitActor->GetActorLocation() - OwnerCharacter->GetActorLocation()).GetSafeNormal();
						const float AngleDifference = FMath::RadiansToDegrees(FMath::Acos(ViewPointRotation.Vector().Dot(ControllerToActorVector))); 
						if (AngleDifference < MinAngle)
						{
							MinAngle = AngleDifference;
							MinAngleActor = HitActor;
						}
					}
				}
			}
		}
		if (MinAngleActor)
		{
			// Target actor selected.
			ControlComponent->SetLockOnTargetActor(MinAngleActor);
			return true;
		}
	}
	// There is no target actor.
	return false;
}

bool UPlayerControlState::IsActorInViewport(const FVector& ActorLocation) const
{
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	
	FVector2D ScreenLocation;
	const bool bIsOnScreen = UGameplayStatics::ProjectWorldToScreen(
			Cast<APlayerController const>(OwnerCharacter->GetController()),
			ActorLocation,
			ScreenLocation
		);

	if (!bIsOnScreen) return false;
	
	return ScreenLocation.X >= 0 && ScreenLocation.X <= ViewportSize.X &&
		   ScreenLocation.Y >= 0 && ScreenLocation.Y <= ViewportSize.Y;
}

void UPlayerControlState::ConsumeItemAction_1()
{
	Super::ConsumeItemAction_1();
	if (ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (UItemComponent* ItemComponent = DomiCharacter->FindComponentByClass<UItemComponent>())
		{
			ItemComponent->UseConsumableItem(0); // 슬롯 인덱스 0
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_1 : ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_1 : OwnerCharacter가 DomiCharacter가 아닙니다."));
	}

	Debug::Print(TEXT("UPlayerControlState::ConsumeItemAction_1 : Call."));
}

void UPlayerControlState::ConsumeItemAction_2()
{
	Super::ConsumeItemAction_2();
	if (ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (UItemComponent* ItemComponent = DomiCharacter->FindComponentByClass<UItemComponent>())
		{
			ItemComponent->UseConsumableItem(1); // 슬롯 인덱스 1 
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_2 : ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_2 : OwnerCharacter가 DomiCharacter가 아닙니다."));
	}

	Debug::Print(TEXT("UPlayerControlState::ConsumeItemAction_2 : Call."));
}

void UPlayerControlState::ConsumeItemAction_3()
{
	Super::ConsumeItemAction_3();
	if (ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (UItemComponent* ItemComponent = DomiCharacter->FindComponentByClass<UItemComponent>())
		{
			ItemComponent->UseConsumableItem(2); // 슬롯 인덱스 2 사용
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_3 : ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::ConsumeItemAction_3 : OwnerCharacter가 DomiCharacter가 아닙니다."));
	}

	Debug::Print(TEXT("UPlayerControlState::ConsumeItemAction_3 : Call."));
}

void UPlayerControlState::SwapWeapon()
{
	Super::SwapWeapon();
	if (ADomiCharacter* DomiCharacter = Cast<ADomiCharacter>(OwnerCharacter))
	{
		if (UItemComponent* ItemComponent = DomiCharacter->FindComponentByClass<UItemComponent>())
		{
			ItemComponent->SwapWeapons();
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlState::SwapWeaponsAction : ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlState::SwapWeaponsAction : OwnerCharacter가 DomiCharacter가 아닙니다."));
	}
	Debug::Print(TEXT("UPlayerControlState::SwapWeapon : Call."));
}

void UPlayerControlState::InteractionScroll(const FInputActionValue& Value)
{
	Super::InteractionScroll(Value);

	auto* PlayerCharacter = Cast<ADomiCharacter>(OwnerCharacter);
	if (PlayerCharacter)
	{
		const float InputValue = Value.Get<float>();
		PlayerCharacter->EventInteractionWidgetScroll(InputValue);
	}

}
