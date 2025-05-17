#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Math/UnrealMathUtility.h"

ACurvedProjectile::ACurvedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(SphereCollision);
}

void ACurvedProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ACurvedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetActorLocation() != TargetLocation)
	{
		// CurvePoint랑 TargetLocation DeltaTime마다 ProjectioleSpeed로 보간하며 CurvePoint Set
		// CurvedProjectile Location, Rotation renew
	}
}

void ACurvedProjectile::MidPointCalculator()
{
	float DistanceRatio = FMath::FRandRange(CurveSettings.MinCurvePointDistance, CurveSettings.MaxCurvePointDistance);

	MidPoint = GetActorLocation() + (TargetLocation - GetActorLocation() * DistanceRatio);
}

void ACurvedProjectile::CurveControl()
{
	FVector Direction = (TargetLocation - MidPoint).GetSafeNormal();
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