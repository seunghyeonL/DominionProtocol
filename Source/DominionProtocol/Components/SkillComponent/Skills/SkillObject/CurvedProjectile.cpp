#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "DomiFramework/ObjectPooling/ObjectPoolSubsystem.h"
#include "Player/Damagable.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/Skills/CurvedProjectileSkill.h"


ACurvedProjectile::ACurvedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->InitSphereRadius(10.0f);
	SphereCollision->SetCollisionObjectType(ECC_GameTraceChannel1); // ECC_GameTraceChannel1 = Projectile
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetGenerateOverlapEvents(true);
	RootComponent = SphereCollision;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(SphereCollision);
	Projectile->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 오브젝트 풀링하면 삭제
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("'/Engine/VREditor/TransformGizmo/TranslateArrowHandle.TranslateArrowHandle'"));
	if (MeshAsset.Succeeded())
	{
		Projectile->SetStaticMesh(MeshAsset.Object);
	}
	//UE_LOG(LogTemp, Warning, TEXT("Spawned Actor Location: %s"), *GetActorLocation().ToString());
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
	if (!bIsInitialize) return;
	if (!IsValid(InstigatorPawn)) return;

	// 이동 로직
	if (GetActorLocation() != TargetPoint)
	{
		if (bIsTargetMove)
		{
			TargetPoint = TargetActor->GetActorLocation();
		}
		CurvePoint = UKismetMathLibrary::VInterpTo_Constant(CurvePoint, TargetPoint, DeltaTime, CurveSettings.ProjectileSpeed);

		SetActorLocation(UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), CurvePoint, DeltaTime, CurveSettings.ProjectileSpeed));

		SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UKismetMathLibrary::VInterpTo_Constant(GetActorLocation(), CurvePoint, DeltaTime, CurveSettings.ProjectileSpeed)));
	}
	if (GetActorLocation() == TargetPoint)
	{
	//	ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	//	ObjectPoolSubsystem->ReturnActorToPool(this);
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
		Destroy();
	}
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
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ACurvedProjectile::DestroyProjectile, CurveSettings.LifeSpan, false);
}

void ACurvedProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this)
	{
		return;
	}

	// 같은 클래스(자기들끼리)라면 무시
	if (OtherActor->IsA(ACurvedProjectile::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped Actor: %s, Channel: %d"),
			*OtherActor->GetName(),
			OtherComp->GetCollisionObjectType());
		return;
	}

	UWorld* World = GetWorld();
	check(World)

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	check(BaseGameState)

		if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
		{
			ImpactParticle = SkillData->Particle;
			if (ImpactParticle)
			{
				UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					ImpactParticle,
					GetActorLocation(),
					FRotator::ZeroRotator,
					true
				);
			}
		}

	if (IsValid(SkillOwner))
	{
		UBaseSkill* BaseSkill = SkillOwner->CurrentSkill;
		if (IsValid(BaseSkill) && BaseSkill->GetSkillTag() == SkillTag)
		{
			BaseSkill->ApplyAttackToHitActor(SweepResult, 0);
		}
	}

	//ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	//ObjectPoolSubsystem->ReturnActorToPool(this);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	Destroy();
}

void ACurvedProjectile::MidPointCalculator()
{
	if (!IsValid(InstigatorPawn)) return;

	StartPoint = GetActorLocation();

	if (!IsValid(TargetActor))
	{
		// TargetActor가 없는 경우 카메라 방향으로 설정
		if (IsValid(InstigatorPawn))
		{
			UCameraComponent* Camera = InstigatorPawn->FindComponentByClass<UCameraComponent>();
			if (Camera)
			{
				const FVector CameraLocation = Camera->GetComponentLocation();
				const FVector ForwardVector = Camera->GetForwardVector();
				const float Distance = 2000.0f;
				TargetPoint = CameraLocation + (ForwardVector * Distance);
			}
		}
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
	// 소멸 이펙트

	//ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	//ObjectPoolSubsystem->ReturnActorToPool(this);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	Destroy();
	Debug::PrintError(TEXT("CurvedProjectile was destroied"));
}