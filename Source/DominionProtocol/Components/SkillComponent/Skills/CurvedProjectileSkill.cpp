#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"
#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "DomiFramework/ObjectPooling/ObjectPoolSubsystem.h"
#include "GameFramework/Character.h"
#include "Interface/PawnTagInterface.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"

UCurvedProjectileSkill::UCurvedProjectileSkill()
{
	SkillTag = SkillTags::CurvedProjectile;
}

void UCurvedProjectileSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);

}

void UCurvedProjectileSkill::Execute()
{
	Super::Execute();

	if (!IsValid(OwnerCharacter)) return;

	if (OwnerCharacter->GetClass()->ImplementsInterface(UPawnTagInterface::StaticClass()))
	{
		FGameplayTag Tag = IPawnTagInterface::Execute_GetPawnTag(OwnerCharacter);
		if (Tag == PawnTags::Player)
		{
			SetTargetActorInPublicSpace();
			if (TargetActor)
			{
				TargetLocation = TargetActor->GetActorLocation();
			}
			else
			{
				UCameraComponent* Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
				if (Camera)
				{
					const FVector CameraLocation = Camera->GetComponentLocation();
					const FVector ForwardVector = Camera->GetForwardVector();
					const float Distance = 1400.0f;
					TargetLocation = CameraLocation + (ForwardVector * Distance);
				}
				else
				{
					TargetLocation = OwnerCharacter->GetActorLocation() + (OwnerCharacter->GetActorForwardVector() * 1400.f);
				}
			}
		}
		else
		{
			ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			check(Player);
			{
				TargetLocation = Player->GetActorLocation();
			}
		}
	}

	ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	if (!IsValid(ObjectPoolSubsystem)) return;

	// 타이머로 순차 발사
	GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &UCurvedProjectileSkill::LaunchNextProjectile, LaunchInterval, true);
	Debug::PrintError(TEXT("CurvedProjectile Lauched"));
}


void UCurvedProjectileSkill::LaunchNextProjectile()
{
	if (ProjectileIndexToLaunch >= TotalProjectileCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
		return;
	}

	// 풀에서 투사체 가져오기
	ACurvedProjectile* CurvedProjectile = ObjectPoolSubsystem->SpawnActorFromPool<ACurvedProjectile>(SpawnLocation, SpawnRotator, OwnerCharacter);

	if (!IsValid(CurvedProjectile)) return;

	SpawnLocation = OwnerCharacter->GetActorLocation();
	SpawnRotator = OwnerCharacter->GetControlRotation();

	// 필요 시 커브 설정 전달
	// FProjectileCurveSettings CustomSettings = ...;
	// CurvedProjectile->SetCurveSettings(CustomSettings);

	CurvedProjectile->Launch(OwnerCharacter, TargetLocation);

	ProjectileIndexToLaunch++;
}

void UCurvedProjectileSkill::SetTargetActorInPublicSpace()
{
	//ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	//check(OwnerCharacter);

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

	SetTargetActor(nullptr);

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
			SetTargetActor(MinDistanceActor);
		}
	}
	// There is no target actor.
}

bool UCurvedProjectileSkill::IsActorInViewport(const FVector& ActorLocation) const
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