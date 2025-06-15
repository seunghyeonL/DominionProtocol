// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControlComponent.h"

#include "ControlComponentUser.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "./States/PlayerControlState.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Effects/BufferedInput/BaseBufferedInput.h"
#include "Effects/PlayerConfusedEffect/PlayerConfusedEffect.h"
#include "Effects/PlayerDeathEffect/PlayerDeathEffect.h"
#include "Effects/PlayerLockOnEffect/PlayerLockOnEffect.h"
#include "Effects/PlayerSilenceEffect/PlayerSilenceEffect.h"
#include "Effects/PlayerStiffnessEffect/PlayerStiffnessEffect.h"
#include "Effects/PlayerStunEffect/PlayerStunEffect.h"
#include "Effects/PlayerUsingSkillEffect/PlayerUsingSkillEffect.h"
#include "InputActionValue.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Effects/PlayerConsumingItemEffect/PlayerConsumingItemEffect.h"
#include "Effects/PlayerFlewEffect/PlayerFlewEffect.h"
#include "Effects/PlayerParryEffect/PlayerParryEffect.h"
#include "Effects/PlayerPreStunEffect/PlayerPreStunEffect.h"
#include "Effects/PlayerStandingUpEffect/PlayerStandingUpEffect.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerControlComponent::UPlayerControlComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	
	PlayerControlState = nullptr;
	ValidBufferedInput = nullptr;
	LockOnTargetActor = nullptr;
	// bIsComponentReady = false;
	// ...

	
}

void UPlayerControlComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	TArray<FGameplayTag> ControlEffectTags;
	ControlEffectMap.GetKeys(ControlEffectTags);
	for (auto EffectTag : ControlEffectTags)
	{
		ControlEffectMap[EffectTag]->Deactivate();
	}
}

// Called when the game starts
void UPlayerControlComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerControlComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Create Input State
	PlayerControlState = NewObject<UPlayerControlState>(this);
	if (!IsValid(PlayerControlState))
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::InitializeComponent : Invalid PlayerControlState."));
		return;
	}

	auto OwnerCharacter = Cast<ACharacter>(GetOuter());
	if (!IsValid(OwnerCharacter))
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::InitializeComponent : Invalid OwnerCharacter."));
		return;
	}
	PlayerControlState->SetOwnerCharacter(OwnerCharacter);

	// Initialize Effects 
	ActiveControlEffectTags.Reset();
	ControlEffectMap.Empty();

	// Add ControlEffects to ControlEffectMapper 
	ControlEffectMap.Add(EffectTags::Stun, NewObject<UPlayerStunEffect>(this));
	ControlEffectMap.Add(EffectTags::PreStun, NewObject<UPlayerPreStunEffect>(this));
	ControlEffectMap.Add(EffectTags::Stiffness, NewObject<UPlayerStiffnessEffect>(this));
	ControlEffectMap.Add(EffectTags::Flew, NewObject<UPlayerFlewEffect>(this));
	ControlEffectMap.Add(EffectTags::StandingUp, NewObject<UPlayerStandingUpEffect>(this));
	ControlEffectMap.Add(EffectTags::Silence, NewObject<UPlayerSilenceEffect>(this));     
	ControlEffectMap.Add(EffectTags::Confused, NewObject<UPlayerConfusedEffect>(this));  
	
	ControlEffectMap.Add(EffectTags::Death, NewObject<UPlayerDeathEffect>(this));
	ControlEffectMap.Add(EffectTags::UsingSkill, NewObject<UPlayerUsingSkillEffect>(this));
	ControlEffectMap.Add(EffectTags::Parry, NewObject<UPlayerParryEffect>(this));
	ControlEffectMap.Add(EffectTags::ConsumingItem, NewObject<UPlayerConsumingItemEffect>(this));
	ControlEffectMap.Add(EffectTags::LockOn, NewObject<UPlayerLockOnEffect>(this));
	
	// Effects Initialize
	for (auto& [ControlEffectTag, ControlEffect] : ControlEffectMap)
	{
		ControlEffect->SetOwnerCharacter(OwnerCharacter);
		ControlEffect->Initialize();
	}
	
	// Notify Component Ready
	// if (OnComponentReady.IsBound())
	// {
	// 	OnComponentReady.Execute();
	// }
	// bIsComponentReady = true;
}

// Called every frame
void UPlayerControlComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	if (IsValid(ValidBufferedInput))
	{
		ValidBufferedInput->Operate();
		ValidBufferedInput = nullptr;
	}
	
	PlayerControlState->Tick(DeltaTime);
}

bool UPlayerControlComponent::IsUsingDoubleExecuteSkill()
{
	if (auto UsingSkillEffectBase = ControlEffectMap.Find(EffectTags::UsingSkill))
	{
		auto UsingSkillEffect = Cast<UPlayerUsingSkillEffect>(*UsingSkillEffectBase);
		return UsingSkillEffect->IsDoubleExecuteSkillEffect();
	}

	Debug::PrintError(TEXT("UPlayerControlComponent::IsUsingDoubleExecuteSkill : UsingSkillEffect is not set."));
	return false;
}

bool UPlayerControlComponent::SetLockOnTargetActorInPublicSpace()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	check(OwnerCharacter);
	
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
		ECC_Pawn,
		FCollisionShape::MakeSphere(PublicSpaceDistance),
		SphereTraceQueryParams
	);

	// SetLockOnTargetActor(nullptr);
	
	if (bHit)
	{
		float MinDistance = PublicSpaceDistance;
		AActor* MinDistanceActor = nullptr;
		for (const FHitResult& Hit : SphereTraceHitResults)
		{
			AActor* HitActor = Hit.GetActor();
			// StatusComponent를 가지고 있는지 == Pawn 중에 Enemy인 액터들만 통과
			auto StatusComponenetUser = Cast<IStatusComponentUser>(HitActor);
			if (!StatusComponenetUser)
			{
				continue;
			}

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
			SetLockOnTargetActor(MinDistanceActor);
			return true;
		}
	}
	// There is no target actor.
	return false;
}

bool UPlayerControlComponent::SetLockOnTargetActorInVisibility()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	check(OwnerCharacter);
	
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
		ECC_Pawn,
		FCollisionShape::MakeBox(BoxSize),
		QueryParams
	);

	SetLockOnTargetActor(nullptr);
	
	if (bBoxTraceHit)
	{
		float MinAngle = 90.f;
		AActor* MinAngleActor = nullptr;
		for (const FHitResult& Hit : SphereTraceHitResults)
		{
			AActor* HitActor = Hit.GetActor();
			// StatusComponent를 가지고 있는지 == Pawn 중에 Enemy인 액터들만 통과
			auto StatusComponenetUser = Cast<IStatusComponentUser>(HitActor);
			if (!StatusComponenetUser)
			{
				continue;
			}

			if (APawn* HitPawn = Cast<APawn>(Hit.GetActor()))
			{
				if (IsActorInViewport(HitActor->GetActorLocation()))
				{
					FHitResult LineTraceHit;
					bool bLineTraceHit = GetWorld()->LineTraceSingleByChannel(
						LineTraceHit,
						OwnerCharacter->GetActorLocation(),
						HitActor->GetActorLocation(),
						ECC_Pawn,
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
			SetLockOnTargetActor(MinAngleActor);
			return true;
		}
	}
	// There is no target actor.
	return false;
}

bool UPlayerControlComponent::IsActorInViewport(const FVector& ActorLocation) const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	check(OwnerCharacter);
	
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


TArray<FEffectUIData> UPlayerControlComponent::GetEffectUIDatas()
{
	TArray<FEffectUIData> EffectUIDatas;
	FGameplayTagContainer BuffDebuffContainer;
	
	BuffDebuffContainer.AddTag(EffectTags::ControlBuff);
	BuffDebuffContainer.AddTag(EffectTags::ControlDebuff);
	
	for (const auto& [EffectTag, ControlEffect] : ControlEffectMap)
	{
		if (EffectTag.MatchesAny(BuffDebuffContainer))
		{
		    if(ControlEffect->IsActive())
		    {
		        EffectUIDatas.Add(ControlEffect->GetEffectUIData());
		    }
		}
	}

	return EffectUIDatas;
}

void UPlayerControlComponent::ActivateControlEffect(const FGameplayTag& ControlEffectTag)
{
	if (ControlEffectTag.MatchesTag(EffectTags::UsingSkill))
	{
		if (auto ControlEffect = ControlEffectMap.Find(EffectTags::UsingSkill))
		{
			if (auto UsingSkillEffect = Cast<UPlayerUsingSkillEffect>(*ControlEffect))
			{
				UsingSkillEffect->SetControlEffectTag(ControlEffectTag);
				UsingSkillEffect->Activate();
			}
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffect : UsingSkill Tag Not Initialized in Mapper."));
		}
		
		return;
	}
	
	if (auto ControlEffect = ControlEffectMap.Find(ControlEffectTag))
	{
		(*ControlEffect)->Activate();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffect : Tag Not Initialized in Mapper."));
	}
}

void UPlayerControlComponent::ActivateControlEffect(const FGameplayTag& ControlEffectTag, float Duration)
{
	if (FMath::IsNearlyZero(Duration))
	{
		ActivateControlEffect(ControlEffectTag);
		return;
	}
	
	if (ControlEffectTag.MatchesTag(EffectTags::UsingSkill))
	{
		if (auto ControlEffect = ControlEffectMap.Find(EffectTags::UsingSkill))
		{
			if (auto UsingSkillEffect = Cast<UPlayerUsingSkillEffect>(*ControlEffect))
			{
				UsingSkillEffect->SetControlEffectTag(ControlEffectTag);
				UsingSkillEffect->Activate(Duration);
			}
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffect : UsingSkill Tag Not Initialized in Mapper."));
		}
		
		return;
	}
	
	if (auto ControlEffect = ControlEffectMap.Find(ControlEffectTag))
	{
		(*ControlEffect)->Activate(Duration);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ActivateControlEffectWithDuration : Tag Not Initialized in Mapper."));
	}
}

void UPlayerControlComponent::DeactivateControlEffect(const FGameplayTag& ControlEffectTag)
{
	if (ControlEffectTag.MatchesTag(EffectTags::UsingSkill))
	{
		if (auto ControlEffect = ControlEffectMap.Find(EffectTags::UsingSkill))
		{
			if (auto UsingSkillEffect = Cast<UPlayerUsingSkillEffect>(*ControlEffect))
			{
				UsingSkillEffect->SetControlEffectTag(ControlEffectTag);
				UsingSkillEffect->Deactivate();
			}
		}
		else
		{
			Debug::PrintError(TEXT("UPlayerControlComponent::DeactivateControlEffect : UsingSkill Tag Not Initialized in Mapper."));
		}
		
		return;
	}
	
	if (auto ControlEffect = ControlEffectMap.Find(ControlEffectTag))
	{
		(*ControlEffect)->Deactivate();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::DeactivateControlEffect : Tag Not Initialized in Mapper."));
	}
}

// Input Binding Functions
void UPlayerControlComponent::Move(const FInputActionValue& Value)
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Move(Value);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Move : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Look(const FInputActionValue& Value)
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Look(Value);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Look : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Dash()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Dash();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Dash : Invalid ControlState."));
	}
}

void UPlayerControlComponent::SprintStart()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->SprintStart();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Sprint : Invalid ControlState."));
	}
}

void UPlayerControlComponent::SprintEnd()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->SprintEnd();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Sprint : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Parry()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Parry();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Parry : Invalid ControlState."));
	}
}

void UPlayerControlComponent::BaseAttack()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->BaseAttack();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::BaseAttack : Invalid ControlState."));
	}
}

void UPlayerControlComponent::WeaponSkill()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->WeaponSkill();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::WeaponSkill : Invalid ControlState."));
	}
}

void UPlayerControlComponent::MagicSkill()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->MagicSkill();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::MagicSkill : Invalid ControlState."));
	}
}

void UPlayerControlComponent::Interact()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->Interact();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Interact : Invalid ControlState."));
	}
}

void UPlayerControlComponent::LockOn()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->LockOn();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::LockOn : Invalid ControlState."));
	}
}

void UPlayerControlComponent::ConsumeItemAction_1()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->ConsumeItemAction_1();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ConsumeItemAction_1 : Invalid ControlState."));
	}
}

void UPlayerControlComponent::ConsumeItemAction_2()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->ConsumeItemAction_2();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ConsumeItemAction_2 : Invalid ControlState."));
	}
}

void UPlayerControlComponent::ConsumeItemAction_3()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->ConsumeItemAction_3();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::ConsumeItemAction_3 : Invalid ControlState."));
	}
}

void UPlayerControlComponent::SwapWeapon()
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->SwapWeapon();
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::SwapWeapon : Invalid ControlState."));
	}
}

void UPlayerControlComponent::InteractionScroll(const FInputActionValue& Value)
{
	if (IsValid(PlayerControlState))
	{
		PlayerControlState->InteractionScroll(Value);
	}
	else
	{
		Debug::PrintError(TEXT("UPlayerControlComponent::Interaction : Invalid ControlState."));
	}
}
