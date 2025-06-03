#include "Components/SkillComponent/Skills/TeleportSkill.h"
#include "Engine/EngineTypes.h" 
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"  
#include "GameFramework/Character.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/Skills/SkillObject/Portal.h"

UTeleportSkill::UTeleportSkill()
{
	SkillTag = SkillTags::MagicTeleportSkill;
	ControlEffectTag = EffectTags::UsingTeleport;
	bReadyToTeleport = false;
	bCanTeleport = false;
}

void UTeleportSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UTeleportSkill::Execute()
{
	//Super::Execute();

	if (!bReadyToTeleport)
	{
		bReadyToTeleport = true;

		UWorld* World = GetWorld();
		if (!World) return;

		FVector SpawnLocation = OwnerCharacter->GetActorLocation();
		FRotator SpawnRotation = OwnerCharacter->GetActorRotation();

		auto SpawnedPortal = World->SpawnActor<APortal>(APortal::StaticClass(), SpawnLocation, SpawnRotation);
		SpawnedPortal->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

		Portal = SpawnedPortal;
		Portal->SkillOwner = this;

		return;
	}
	
	Move();
}

void UTeleportSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	OwnerLocation = OwnerCharacter->GetActorLocation();

	UWorld* World = GetWorld();
	if (!World) return;

	// 텔레포트 조준 모드가 활성화된 경우에만 실행
	if (bReadyToTeleport)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(OwnerCharacter);

		// 캐릭터의 조준 방향을 가져와 Yaw 반영하여 회전 조정
		FRotator AimRotation = OwnerCharacter->GetControlRotation();
		FRotator AdjustedRotation = FRotator(0.f, AimRotation.Yaw, 0.f);

		Portal->GetStartPoint()->SetWorldRotation(AdjustedRotation);
		Portal->GetMovePoint()->SetWorldRotation(AdjustedRotation);

		const FVector Start = Portal->GetStartPoint()->GetComponentLocation();
		const FVector End = Portal->GetEndPoint()->GetComponentLocation();

		// 캐릭터가 바라보는 방향에 벽이 있는지 확인
		FHitResult ForwardAimHitResult;
		bool bForwardHit = World->LineTraceSingleByChannel(
			ForwardAimHitResult,
			Start,
			End,
			ECC_Visibility,
			Params
		);

		//DrawDebugLine(
		//	World,
		//	Start,
		//	End,
		//	FColor::White,
		//	false,
		//	2.0f,
		//	0,
		//	2.0f
		//);

		// 벽 발견
		if (bForwardHit)
		{
			// 충돌 지점으로 이동 포인트 설정
			Portal->GetMovePoint()->SetWorldLocation(ForwardAimHitResult.ImpactPoint);

			// 벽 위치에서 안전거리만큼 뒤로 이동한 지점 아래 바닥 확인
			const FVector ForwardHitLocation = Portal->GetMovePoint()->GetComponentLocation();
			const FVector GroundCheckStart1 = ForwardHitLocation + (Portal->GetMovePoint()->GetForwardVector() * -ForwardOffsetDistance);
			const FVector GroundCheckEnd1 = GroundCheckStart1 + FVector(0.f, 0.f, -DownTraceDistance);

			FHitResult GroundBackHitResult1;
			bool bGroundBackHit = World->LineTraceSingleByChannel(
				GroundBackHitResult1,
				GroundCheckStart1,
				GroundCheckEnd1,
				ECC_Visibility,
				Params
			);

			//DrawDebugLine(
			//	World,
			//	GroundCheckStart1,
			//	GroundCheckEnd1,
			//	FColor::Green,
			//	false,
			//	2.0f,
			//	0,
			//	2.0f
			//);

			// 착지 위치 후보 1 (벽 있을 때)
			FVector CandidateLocation = GroundBackHitResult1.ImpactPoint;
			FVector InitialHitPoint = ForwardAimHitResult.ImpactPoint;

			// 플레이어와 HitPoint 사이 경사, 후보 1과 HitPoint 사이 경사, 플레이어와 후보 1 사이 거리 체크)
			if (IsValidAngle(OwnerLocation, InitialHitPoint) && IsValidAngle(CandidateLocation, InitialHitPoint) && IsValidDistance(CandidateLocation))
			{
				Portal->GetBlueAura()->SetWorldLocation(GroundBackHitResult1.ImpactPoint);
				ActivateBlue();
			}
			else
			{
				Portal->GetRedAura()->SetWorldLocation(GroundBackHitResult1.ImpactPoint);
				ActivateRed();
			}
		}
		// 벽 없음 - 조준 방향 끝점에서 아래로 바닥 탐색
		else
		{
			const FVector ForwardEndGroundCheckStart = Portal->GetEndPoint()->GetComponentLocation();
			const FVector ForwardEndGroundCheckEnd = ForwardEndGroundCheckStart + FVector(0.f, 0.f, -DownTraceDistance);

			FHitResult DirectDownwardHitResult;
			bool bDirectHit = World->LineTraceSingleByChannel(
				DirectDownwardHitResult,
				ForwardEndGroundCheckStart,
				ForwardEndGroundCheckEnd,
				ECC_Visibility,
				Params
			);

			//DrawDebugLine(
			//	World,
			//	ForwardEndGroundCheckStart,
			//	ForwardEndGroundCheckEnd,
			//	FColor::Yellow,
			//	false,
			//	2.0f,
			//	0,
			//	2.0f
			//);

			// 바닥 발견
			if (bDirectHit)
			{
				// 라인트레이스 끝 지점에서 찾은 바닥과 안전거리만큼 뒤로 이동한 지점에서 찾은 바닥 사이 경사 체크
				FVector ForwardDirection = Portal->GetStartPoint()->GetForwardVector();
				const FVector GroundCheckStart2 = ForwardAimHitResult.TraceEnd - ForwardDirection * ForwardOffsetDistance;
				const FVector GroundCheckEnd2 = GroundCheckStart2 + FVector(0.f, 0.f, -DownTraceDistance);

				FHitResult SafeBackwardGroundHitResult;
				bool bGroundBackHit = World->LineTraceSingleByChannel(
					SafeBackwardGroundHitResult,
					GroundCheckStart2,
					GroundCheckEnd2,
					ECC_Visibility,
					Params
				);

				//DrawDebugLine(
				//	World,
				//	GroundCheckStart2,
				//	GroundCheckEnd2,
				//	FColor::Green,
				//	false,
				//	2.0f,
				//	0,
				//	2.0f
				//);

				// 착지 위치 후보 2 (벽 없을 때)
				FVector AlternateLocation = SafeBackwardGroundHitResult.ImpactPoint;
				FVector InitialDownwardHitPoint = DirectDownwardHitResult.ImpactPoint;

				// 플레이어와 HitPoint 사이 경사, 후보 2와 HitPoint 사이 경사, 플레이어와 후보 2 사이 거리 체크
				if (IsValidAngle(OwnerLocation, InitialDownwardHitPoint) && IsValidAngle(AlternateLocation, InitialDownwardHitPoint) && IsValidDistance(AlternateLocation))
				{
					Portal->GetBlueAura()->SetWorldLocation(AlternateLocation);
					ActivateBlue();
				}
				else
				{
					Portal->GetRedAura()->SetWorldLocation(AlternateLocation);
					ActivateRed();
				}
			}
			// 안전한 바닥 없음 - 텔레포트 불가 (낭떠러지 등)
			else
			{
				Portal->GetRedAura()->SetWorldLocation(DirectDownwardHitResult.TraceEnd);
				ActivateRed();
			}
		}
	}
}

void UTeleportSkill::Move()
{
	if (bCanTeleport)
	{
		OwnerCharacter->TeleportTo(Portal->GetBlueAura()->GetComponentLocation() + FVector(0, 0, 100.0f), OwnerCharacter->GetActorRotation());
		bReadyToTeleport = false;
		bCanTeleport = false;
		Portal->GetBlueAura()->SetVisibility(false);

		USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

		check(SkillComponent);
		SkillComponent->EndSkill();
	}
}

bool UTeleportSkill::IsValidAngle(const FVector& StartLocation, const FVector& TargetLocation)
{
	const FVector DirectionVector = TargetLocation - StartLocation;
	const float HorizontalDistance = FVector(DirectionVector.X, DirectionVector.Y, 0.f).Size();
	const float VerticalDistance = FMath::Abs(DirectionVector.Z);
	const float SlopeAngleDegrees = FMath::RadiansToDegrees(FMath::Atan2(VerticalDistance, HorizontalDistance));

	return SlopeAngleDegrees <= MaxSlopeAngle;
}

bool UTeleportSkill::IsValidDistance(const FVector& StartLocation)
{
	const float TotalDistance = FVector::Dist(OwnerLocation, StartLocation);

	return TotalDistance <= MaxTeleportDistance;
}

void UTeleportSkill::ActivateBlue()
{
	Portal->GetBlueAura()->SetVisibility(true);
	Portal->GetRedAura()->SetVisibility(false);

	bCanTeleport = true;
}

void UTeleportSkill::ActivateRed()
{
	Portal->GetBlueAura()->SetVisibility(false);
	Portal->GetRedAura()->SetVisibility(true);

	bCanTeleport = false;
}