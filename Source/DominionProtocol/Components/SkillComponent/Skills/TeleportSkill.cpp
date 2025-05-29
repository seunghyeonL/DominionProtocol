#include "Components/SkillComponent/Skills/TeleportSkill.h"
#include "Engine/EngineTypes.h" 
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"  
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"

UTeleportSkill::UTeleportSkill()
{
	SkillTag = SkillTags::TeleportSkill;
	Distance = 500.0f;
}

void UTeleportSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UTeleportSkill::Execute()
{
	//Super::Execute();

	Start();
}

void UTeleportSkill::Start()
{
	UCameraComponent* Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	FVector PlayerForward = OwnerCharacter->GetActorForwardVector();
	PlayerForward.Z = 0.f;
	PlayerForward.Normalize();

	FVector StartLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = StartLocation + PlayerForward * Distance;

	FRotator PlayerYawRotation = OwnerCharacter->GetActorRotation();
	PlayerYawRotation.Pitch = 0.f;
	PlayerYawRotation.Roll = 0.f;

	UWorld* World = GetWorld();
	check(World);

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	check(BaseGameState);

	if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
	{
		Sound[0] = SkillData->Sound[0];
		if (Sound[0])
		{
			UGameplayStatics::PlaySoundAtLocation(this, Sound[0], StartLocation);
		}

		Particle[0] = SkillData->Particle[0];
		if (Particle[0])
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle[0], StartLocation, FRotator::ZeroRotator, true);
		}
	}

	FHitResult Hit;
	bool bMoved = OwnerCharacter->SetActorLocation(TargetLocation, true, &Hit);

	if (!bMoved && Hit.bBlockingHit)
	{
		float SlopeAngleDeg = FMath::RadiansToDegrees(acosf(Hit.ImpactNormal.Z));

		if (SlopeAngleDeg <= 45.f) // 경사면
		{
			// 경사면 위를 따라 이동할 방향 (경사면의 접선 방향)
			FVector ForwardOnSlope = FVector::VectorPlaneProject(PlayerForward, Hit.ImpactNormal).GetSafeNormal();

			// 경사면을 따라 Distance만큼 이동한 목표 위치
			FVector SlideTarget = StartLocation + ForwardOnSlope * Distance;

			// 경사면 방향으로 Sweep 이동 시도
			FHitResult SlideHit;
			bool bSlideMoved = OwnerCharacter->SetActorLocation(SlideTarget, true, &SlideHit);

			if (!bSlideMoved)
			{
				// 실패하면 장애물 위로 순간이동
				FVector OverObstacle = Hit.ImpactPoint + FVector(0, 0, 100.f);
				OwnerCharacter->SetActorLocation(OverObstacle, false);
			}
		}
		else // 수직 장애물로 판단
		{
			FVector OverObstacle = Hit.ImpactPoint + FVector(0, 0, 100.f); // 위로 100 이동
			OwnerCharacter->SetActorLocation(OverObstacle, false); // 강제 이동
		}
	}

	OwnerCharacter->SetActorRotation(PlayerYawRotation);

	GetWorld()->GetTimerManager().SetTimer(TeleportTimerHandle, this, &UTeleportSkill::End, 0.1f, false);
}


void UTeleportSkill::End()
{
	UWorld* World = GetWorld();
	check(World);

	FVector SpawnLocation = OwnerCharacter->GetActorLocation();

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	check(BaseGameState);

	if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
	{
		Particle[0] = SkillData->Particle[0];
		if (Particle[0])
		{
			UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				Particle[0],
				SpawnLocation,
				FRotator::ZeroRotator,
				true
			);
		}
	}

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}