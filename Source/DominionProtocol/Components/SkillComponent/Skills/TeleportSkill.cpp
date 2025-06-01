#include "Components/SkillComponent/Skills/TeleportSkill.h"
#include "Engine/EngineTypes.h" 
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"  
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Player/Characters/DomiCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"

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

	DomiChar = Cast<ADomiCharacter>(OwnerCharacter);
	check(DomiChar);

	StartPoint = DomiChar->GetStartPoint();
	BeginTrace = DomiChar->GetBeginTrace();
	MovePoint = DomiChar->GetMovePoint();
	Aura = DomiChar->GetTeleportAura();
	CantAura = DomiChar->GetCantTeleportAura();

	if (!bReadyToTeleport)
	{
		bReadyToTeleport = true;

		return;
	}
	
	Move();
}

void UTeleportSkill::Move()
{
	if (bCanTeleport)
	{
		OwnerCharacter->TeleportTo(Aura->GetComponentLocation() + FVector(0, 0, 100.0f), OwnerCharacter->GetActorRotation());
		bReadyToTeleport = false;
		bCanTeleport = false;
		Aura->SetVisibility(false);
		CantAura->SetVisibility(false);

		USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

		check(SkillComponent);
		SkillComponent->EndSkill();
	}
}

void UTeleportSkill::Tick(float DeltaTime)
{
	UWorld* World = GetWorld();
	if (!World) return;

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
	if (!SkillComponent->GetCurrentSkill()) return;

	// 텔레포트 조준 모드가 활성화된 경우에만 실행
	if (bReadyToTeleport)
	{
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(DomiChar);

		// 캐릭터의 조준 방향을 가져와 Yaw 반영하여 회전 조정
		FRotator AimRotation = DomiChar->GetControlRotation();
		FRotator AdjustedRotation = FRotator(0.f, AimRotation.Yaw, 0.f);

		StartPoint->SetWorldRotation(AdjustedRotation);
		MovePoint->SetWorldRotation(AdjustedRotation);

		const FVector Start = StartPoint->GetComponentLocation();
		const FVector End = BeginTrace->GetComponentLocation();

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
			MovePoint->SetWorldLocation(ForwardAimHitResult.ImpactPoint);

			// 약간 앞쪽으로 안전하게 텔레포트할 수 있는지 확인
			const FVector SafeLandingLocation = MovePoint->GetComponentLocation();
			const FVector AdjustedBackwardLocation = SafeLandingLocation + (MovePoint->GetForwardVector() * -BackwardOffsetDistance);

			FHitResult BackStepAdjustResult;
			bool bBackHit = World->LineTraceSingleByChannel(
				BackStepAdjustResult,
				SafeLandingLocation,
				AdjustedBackwardLocation,
				ECC_Visibility,
				Params
			);

			//DrawDebugLine(
			//	World,
			//	SafeLandingLocation,
			//	AdjustedBackwardLocation,
			//	FColor::Blue,
			//	false,
			//	2.0f,
			//	0,
			//	2.0f
			//);

			// 벽 위치에서 안전거리만큼 앞으로 이동한 지점 아래 바닥 확인
			const FVector GroundBackCheckStart = AdjustedBackwardLocation;
			const FVector GroundBackCheckEnd = GroundBackCheckStart + FVector(0.f, 0.f, -DownTraceLength);

			FHitResult GroundBackHitResult;
			bool bGroundBackHit = World->LineTraceSingleByChannel(
				GroundBackHitResult,
				GroundBackCheckStart,
				GroundBackCheckEnd,
				ECC_Visibility,
				Params
			);

			//DrawDebugLine(
			//	World,
			//	GroundBackCheckStart,
			//	GroundBackCheckEnd,
			//	FColor::Green,
			//	false,
			//	2.0f,
			//	0,
			//	2.0f
			//);

			Aura->SetWorldLocation(GroundBackHitResult.ImpactPoint);

			Aura->SetVisibility(true);
			CantAura->SetVisibility(false);

			bCanTeleport = true;
		}
		// 벽 없음 - 조준 방향 끝점에서 직접 바닥 탐색
		else
		{
			const FVector DirectTraceStart = BeginTrace->GetComponentLocation();
			const FVector DirectTraceEnd = DirectTraceStart + FVector(0.f, 0.f, -DownTraceLength);

			FHitResult DirectDownwardHitResult;
			bool bDirectHit = World->LineTraceSingleByChannel(
				DirectDownwardHitResult,
				DirectTraceStart,
				DirectTraceEnd,
				ECC_Visibility,
				Params
			);

			if (bDirectHit)
			{
				// 조준 방향에 안전한 바닥 발견 - 직접 텔레포트 가능
				Aura->SetWorldLocation(DirectDownwardHitResult.ImpactPoint);

				Aura->SetVisibility(true);
				CantAura->SetVisibility(false);

				bCanTeleport = true;
			}
			else
			{
				// 바닥 없음 - 텔레포트 불가 (낭떠러지 등)
				CantAura->SetWorldLocation(DirectDownwardHitResult.ImpactPoint);

				Aura->SetVisibility(false);
				CantAura->SetVisibility(true);

				bCanTeleport = false;
			}
		}
	}
}