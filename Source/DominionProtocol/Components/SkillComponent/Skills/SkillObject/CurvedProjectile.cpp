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


ACurvedProjectile::ACurvedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	//SphereCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
	//SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//SphereCollision->SetGenerateOverlapEvents(true);
	RootComponent = SphereCollision;

	Projectile = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	Projectile->SetupAttachment(SphereCollision);

	// 오브젝트 풀링하면 삭제
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("'/Engine/VREditor/TransformGizmo/TranslateArrowHandle.TranslateArrowHandle'"));
	//if (MeshAsset.Succeeded())
	//{
	//	Projectile->SetStaticMesh(MeshAsset.Object);
	//}
	//UE_LOG(LogTemp, Warning, TEXT("Spawned Actor Location: %s"), *GetActorLocation().ToString());
}

void ACurvedProjectile::SetOwnerCharacter(AActor* NewOwnerCharacter)
{
	OwnerCharacter = NewOwnerCharacter;
	SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();
}

void ACurvedProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!bIsActivate) return;
	if (!bIsInitialize) return;
	if (!IsValid(Instigator)) return;

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
		ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
		ObjectPoolSubsystem->ReturnActorToPool(this);
	}
}

void ACurvedProjectile::OnObjectSpawn_Implementation()
{
	Super::OnObjectSpawn_Implementation();
	
	Debug::PrintError(TEXT("CurvedProjectile is Spawned"));

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACurvedProjectile::OnOverlapBegin);

	Debug::PrintError(TEXT("ACurvedProjectile::OnOverlapBegin Bind"));
}

void ACurvedProjectile::OnObjectReturn_Implementation()
{
	Super::OnObjectReturn_Implementation();
	
	Debug::PrintError(TEXT("CurvedProjectile is return"));

	SphereCollision->OnComponentBeginOverlap.RemoveDynamic(this, &ACurvedProjectile::OnOverlapBegin);
	Debug::PrintError(TEXT("ACurvedProjectile::OnOverlapBegin UnBind"));
}

void ACurvedProjectile::SetLaunchPath(AActor* NewInstigator, AActor* NewTargetActor)
{
	if (!IsValid(NewInstigator)) return;

	Instigator = NewInstigator;
	TargetActor = NewTargetActor;

	// 중간 지점 및 곡선 지점 계산
	MidPointCalculator();
	CurveControl();

	bIsInitialize = true;

	//check(GetWorld());
	//GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ACurvedProjectile::DestroyProjectile, CurveSettings.LifeSpan, false);
}

void ACurvedProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UWorld* World = GetWorld();
	check(World)

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	check(BaseGameState)

	if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTags::CurvedProjectile))
	{
		ensure(!SkillData->Particle);

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

	if (!IsValid(SkillComponent)) return;
	UBaseSkill* BaseSkill = SkillComponent->GetCurrentSkill();

	if (!IsValid(BaseSkill)) return;
	if (BaseSkill->GetSkillTag() == SkillTags::CurvedProjectile)
	{
		BaseSkill->ApplyAttackToHitActor(SweepResult, 0);
	}

	ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	ObjectPoolSubsystem->ReturnActorToPool(this);

	//// 충돌 파라미터 설정
	//FCollisionQueryParams Params;

	//// 소유 캐릭터 무시
	//Params.AddIgnoredActor(OwnerCharacter);
	//Params.AddIgnoredActor(this);
}

void ACurvedProjectile::MidPointCalculator()
{
	if (!IsValid(Instigator)) return;

	StartPoint = GetActorLocation();

	if (!IsValid(TargetActor))
	{
		// TargetActor가 없는 경우 카메라 방향으로 설정
		if (IsValid(OwnerCharacter))
		{
			UCameraComponent* Camera = OwnerCharacter->FindComponentByClass<UCameraComponent>();
			if (Camera)
			{
				const FVector CameraLocation = Camera->GetComponentLocation();
				const FVector ForwardVector = Camera->GetForwardVector();
				const float Distance = 1000.0f;
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

	ObjectPoolSubsystem = GetWorld()->GetSubsystem<UObjectPoolSubsystem>();
	ObjectPoolSubsystem->ReturnActorToPool(this);
	//this->Destroy();
	Debug::PrintError(TEXT("CurvedProjectile was destroied"));
}
