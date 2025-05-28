#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/Damagable.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"
#include "Interface/Parryable.h"
//#include "DomiFramework/ObjectPooling/ObjectPoolSubsystem.h"

ACurvedProjectile::ACurvedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->InitSphereRadius(30.0f);
	SphereCollision->SetCollisionObjectType(ECC_GameTraceChannel2); // Set ObjectType Projectile
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore DashingPawn
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetGenerateOverlapEvents(true);
	RootComponent = SphereCollision;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(SphereCollision);
	Projectile->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 오브젝트 풀링하면 삭제
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("'/Engine/VREditor/TransformGizmo/TranslateArrowHandle.TranslateArrowHandle'"));
	//if (MeshAsset.Succeeded())
	//{
	//	Projectile->SetStaticMesh(MeshAsset.Object);
	//}
}

void ACurvedProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACurvedProjectile::OnOverlapBegin);
}

void ACurvedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if(!bIsActivate) return;
	if (!bIsInitialize || !IsValid(InstigatorPawn)) return;

	ElapsedTime += DeltaTime;

	// LifeSpan 체크
	if (ElapsedTime >= CurveSettings.LifeSpan)
	{
		DestroyProjectile();
		return;
		//	ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
		//	ObjectPoolSubsystem->ReturnActorToPool(this);
	}

	if (bReachedTarget)
	{
		MoveInStraightLine(DeltaTime);
		return;
	}

	// 타겟 도달 체크
	if (HasReachedTarget())
	{
		OnReachTarget();
		return;
	}

	// 커브 이동
	UpdateCurveMovement(DeltaTime);
}

//void ACurvedProjectile::OnObjectSpawn_Implementation()
//{
//	Super::OnObjectSpawn_Implementation();
//	
//	Debug::PrintError(TEXT("CurvedProjectile is Spawned"));
//
//	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACurvedProjectile::OnOverlapBegin);
//
//	Debug::PrintError(TEXT("ACurvedProjectile::OnOverlapBegin Bind"));
//}
//
//void ACurvedProjectile::OnObjectReturn_Implementation()
//{
//	Super::OnObjectReturn_Implementation();
//	
//	Debug::PrintError(TEXT("CurvedProjectile is return"));
//
//	SphereCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ACurvedProjectile::OnOverlapBegin);
//	Debug::PrintError(TEXT("ACurvedProjectile::OnOverlapBegin UnBind"));
//}

void ACurvedProjectile::SetLaunchPath(AActor* NewInstigator, AActor* NewTargetActor)
{
	if (!IsValid(NewInstigator)) return;

	InstigatorPawn = Cast<APawn>(NewInstigator);
	SetInstigator(InstigatorPawn);

	TargetActor = NewTargetActor;

	// 중간 지점 및 곡선 지점 계산
	MidPointCalculator();
	CurveControl();

	bIsInitialize = true;

	check(GetWorld());
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ACurvedProjectile::DestroyProjectile, CurveSettings.LifeSpan, false);
}

void ACurvedProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 같은 클래스(자기들끼리)라면 무시
	if (OtherActor->IsA(ACurvedProjectile::StaticClass()))	return;
	
	// 사용자 본인은 무시
	if (OtherActor == InstigatorPawn)
	{
		return;
	}

	// 스폰시 InstigatorPawn이 설정되기 이전에 오버랩 무시
	if (!IsValid(InstigatorPawn))
	{
		return;
	}

	// check Parry
	if (CheckParry(SweepResult))
	{
		return;
	}
	
	if (IsValid(SkillOwner))
	{
		AActor* HitActor = SweepResult.GetActor();
		if (auto ParryableTarget = Cast<IParryable>(HitActor))
		{
			if (ParryableTarget->IsParryingCond())
			{
				SetLaunchPath(HitActor, InstigatorPawn);
			}
		}
		
		UBaseSkill* BaseSkill = SkillOwner;
		if (IsValid(BaseSkill) && BaseSkill->GetSkillTag() == SkillTag)
		{
			ApplyAttackToHitActor(SweepResult, 0);
		}
	}

	//ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	//ObjectPoolSubsystem->ReturnActorToPool(this);

	DestroyProjectile();
}

void ACurvedProjectile::ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime)
{
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))
	{
		return;
	}

	check(InstigatorPawn)

	if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
	{
		IDamagable::Execute_OnAttacked(HitActor, AttackData);
	}
}

bool ACurvedProjectile::CheckParry(const FHitResult& HitResult)
{
	check(InstigatorPawn);
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))
	{
		return false;
	}
	
	if (auto ParryableTarget = Cast<IParryable>(HitActor))
	{
		if (ParryableTarget->IsParryingCond())
		{
			OnParried(HitActor);
			return true;
		}
	}

	return false;
}

void ACurvedProjectile::OnParried(AActor* ParryActor)
{
	AttackData.Instigator = ParryActor;
	InstigatorPawn = Cast<APawn>(ParryActor);
	TargetActor = InstigatorPawn;
	bReachedTarget = true;
	DirectionVector = ParryActor->GetActorForwardVector();
	SetActorRotation(DirectionVector.Rotation());
}

void ACurvedProjectile::MidPointCalculator()
{
	if (!IsValid(InstigatorPawn)) return;

	StartPoint = GetActorLocation();

	if (!IsValid(TargetActor))
	{
		if (IsValid(InstigatorPawn))
		{
			// TargetActor가 없는 경우 플레이어 전방 10미터 방향으로 설정
			const FVector OriginLocation = InstigatorPawn->GetActorLocation();
			const FVector ForwardVector = InstigatorPawn->GetActorForwardVector();
			const float Distance = 1000.0f;
			TargetPoint = OriginLocation + (ForwardVector * Distance);
		}
		//if (IsValid(InstigatorPawn))
		//{
		//  // TargetActor가 없는 경우 카메라 전방 10미터 방향으로 설정
		//	UCameraComponent* Camera = InstigatorPawn->FindComponentByClass<UCameraComponent>();
		//	if (Camera)
		//	{
		//		const FVector CameraLocation = Camera->GetComponentLocation();
		//		const FVector ForwardVector = Camera->GetForwardVector();
		//		const float Distance = 1000.0f;
		//		TargetPoint = CameraLocation + (ForwardVector * Distance);
		//	}
		//}
	}
	else
	{
		// TargetActor가 있는 경우
		TargetPoint = TargetActor->GetActorLocation();
		bIsTargetMove = true;
	}

	// 중간 지점 계산
	float DistanceRatio = FMath::FRandRange(CurveSettings.MinCurvePointDistance, CurveSettings.MaxCurvePointDistance);
	MidPoint = StartPoint + (TargetPoint - StartPoint) * DistanceRatio;
}

void ACurvedProjectile::CurveControl()
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(MidPoint, TargetPoint);

	FVector XAxis, YAxis, ZAxis;
	UKismetMathLibrary::BreakRotIntoAxes(LookAtRotation, XAxis, YAxis, ZAxis);

	float RadiusRatio = FMath::FRandRange(CurveSettings.MinCurveRadius, CurveSettings.MaxCurveRadius);
	float AngleRatio = FMath::FRandRange(CurveSettings.MinAngle, CurveSettings.MaxAngle);

	XAxis *= RadiusRatio;
	YAxis *= RadiusRatio;

	CurvePoint = MidPoint + UKismetMathLibrary::RotateAngleAxis(YAxis, AngleRatio, XAxis);
}

void ACurvedProjectile::DestroyProjectile()
{
	UWorld* World = GetWorld();
	check(World);

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	check(BaseGameState);

	if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
	{
		Sound = SkillData->Sound[1];
		if (Sound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				Sound,
				GetActorLocation()
			);
		}

		Particle = SkillData->Particle[0];
		if (Particle)
		{
			UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				Particle,
				GetActorLocation(),
				FRotator::ZeroRotator,
				true
			);
		}
	}

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	Destroy();

	//ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	//ObjectPoolSubsystem->ReturnActorToPool(this);
}

void ACurvedProjectile::UpdateCurveMovement(float DeltaTime)
{
	// 커브 시간 체크 및 타겟 업데이트
	if (ElapsedTime < CurveSettings.CurveDuration && bIsTargetMove)
	{
		TargetPoint = TargetActor->GetActorLocation();
	}
	else if (ElapsedTime >= CurveSettings.CurveDuration && !bCurveFixed)
	{
		FixTargetPoint();
	}

	// 커브 포인트 업데이트 (커브가 고정되기 전까지만)
	if (!bCurveFixed)
	{
		CurvePoint = UKismetMathLibrary::VInterpTo_Constant(CurvePoint, TargetPoint, DeltaTime, CurveSettings.ProjectileSpeed);
	}

	FVector MoveTarget = bCurveFixed ? TargetPoint : CurvePoint;

	// 회전
	if (!bReachedTarget)
	{
		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MoveTarget));
	}
	
	// 이동
	SetActorLocation(UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), MoveTarget, DeltaTime, CurveSettings.ProjectileSpeed));
}

void ACurvedProjectile::FixTargetPoint()
{
	if (bIsTargetMove && IsValid(TargetActor))
	{
		TargetPoint = TargetActor->GetActorLocation();
	}
	bCurveFixed = true;
}

bool ACurvedProjectile::HasReachedTarget()
{
	return GetActorLocation().Equals(TargetPoint, 10.0f); // 10cm 오차 허용
}

void ACurvedProjectile::OnReachTarget()
{
	DirectionVector = GetActorForwardVector().GetSafeNormal();
	bReachedTarget = true;
}

void ACurvedProjectile::MoveInStraightLine(float DeltaTime)
{
	// 방향 벡터로 계속 직진
	FVector NewLocation = GetActorLocation() + (DirectionVector * CurveSettings.ProjectileSpeed * DeltaTime);
	SetActorLocation(NewLocation);
}
