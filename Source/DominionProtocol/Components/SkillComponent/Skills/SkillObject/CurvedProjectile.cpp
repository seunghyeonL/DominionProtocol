#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "DomiFramework/ObjectPooling/ObjectPoolSubsystem.h"
#include "Player/Damagable.h"
#include "Math/UnrealMathUtility.h"

ACurvedProjectile::ACurvedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(SphereCollision);
}

void ACurvedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsActivate) return;

	if (GetActorLocation() != TargetPoint)
	{
		CurvePoint = VInterpToConstant(CurvePoint, TargetPoint, DeltaTime, CurveSettings.ProjectileSpeed);

		SetActorLocation(CurvePoint);

		FRotator LookAtRotation = (TargetPoint - CurvePoint).Rotation();
		SetActorRotation(LookAtRotation);
	}
}

void ACurvedProjectile::OnObjectSpawn_Implementation()
{
	Super::OnObjectSpawn_Implementation();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACurvedProjectile::OnOverlapBegin);
}

void ACurvedProjectile::OnObjectReturn_Implementation()
{
	Super::OnObjectReturn_Implementation();
	
	SphereCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ACurvedProjectile::OnOverlapBegin);
}

void ACurvedProjectile::Launch(AActor* NewInstigator, const FVector& NewTargetPoint)
{
	Instigator = NewInstigator;
	TargetPoint = NewTargetPoint;
	StartPoint = GetActorLocation();

	// 중간 지점 및 곡선 지점 계산
	MidPointCalculator();
	CurveControl();

	check(GetWorld());
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ACurvedProjectile::DestroyProjectile, CurveSettings.LifeSpan, false);
}

void ACurvedProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* HitActor = OtherActor;

	if (!IsValid(HitActor))	return;

	UWorld* World = GetWorld();

	if (!IsValid(World)) return;

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

	if (!IsValid(BaseGameState)) return;

	UStatusComponent* StatusComponent = Instigator->FindComponentByClass<UStatusComponent>();

	if (!IsValid(StatusComponent)) return;
	float AttackPower = StatusComponent->GetStat(StatTags::AttackPower);

	FAttackData AttackData;

	check(Instigator);

	//AttackData.Damage = GetFinalAttackData(AttackPower);

	AttackData.Instigator = Instigator;
	//AttackData.Effects = Effects;

	if (HitActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()))
	{
		AttackData.LaunchVector = HitActor->GetActorLocation() - Instigator->GetActorLocation();

		AttackData.LaunchVector.Normalize();

		IDamagable::Execute_OnAttacked(HitActor, AttackData);
	}
}

void ACurvedProjectile::MidPointCalculator()
{
	float DistanceRatio = FMath::FRandRange(CurveSettings.MinCurvePointDistance, CurveSettings.MaxCurvePointDistance);

	MidPoint = GetActorLocation() + (TargetPoint - GetActorLocation()) * DistanceRatio;
}

void ACurvedProjectile::CurveControl()
{
	FVector Direction = (TargetPoint - MidPoint).GetSafeNormal();
	FRotator LookAtRotation = Direction.Rotation();

	float RadiusRatio = FMath::FRandRange(CurveSettings.MinCurveRadius, CurveSettings.MaxCurveRadius);
	float AngleRatio = FMath::FRandRange(CurveSettings.MinAngle, CurveSettings.MaxAngle);

	FVector XAxis, YAxis, ZAxis;
	FRotationMatrix(LookAtRotation).GetScaledAxes(XAxis, YAxis, ZAxis);

	// Y축 방향 벡터에 곡률 반지름을 곱한 벡터
	FVector InVector = YAxis * RadiusRatio;

	// X축(Pitch 방향)을 기준으로 InVector를 회전
	FVector RotatedVector = InVector.RotateAngleAxis(AngleRatio, XAxis);

	CurvePoint = MidPoint + RotatedVector;
}

FVector ACurvedProjectile::VInterpToConstant(const FVector& Current, const FVector& Target, float DeltaTime, float Speed)
{
	if (Speed <= 0.f || Current == Target)
	{
		return Target;
	}

	FVector Direction = (Target - Current).GetSafeNormal();
	float Distance = FVector::Dist(Current, Target);
	float MoveStep = Speed * DeltaTime;

	if (MoveStep >= Distance)
	{
		return Target;
	}

	return Current + Direction * MoveStep;
}

void ACurvedProjectile::DestroyProjectile()
{
	// 소멸 이펙트

	UObjectPoolSubsystem* ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	ObjectPoolSubsystem->ReturnActorToPool(this);
}
