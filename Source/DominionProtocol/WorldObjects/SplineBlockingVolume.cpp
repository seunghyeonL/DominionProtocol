// Fill out your copyright notice in the Description page of Project Settings.


#include "SplineBlockingVolume.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"

ASplineBlockingVolume::ASplineBlockingVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;
	
	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);
}

void ASplineBlockingVolume::BeginPlay()
{
	Super::BeginPlay();
}

#if WITH_EDITOR
void ASplineBlockingVolume::PostEditUndo()
{
	Super::PostEditUndo();

	ClearBox();
	CreateBox();
}
#endif

void ASplineBlockingVolume::OnConstruction(const FTransform& Transform)
{
	SetFlags(RF_Transactional);
	Modify();
	Super::OnConstruction(Transform);

	for (UBoxComponent* Box : BoxComponents)
	{
		if (IsValid(Box))
		{
			Box->SetFlags(RF_Transactional);
			Box->Modify();
		}
	}
	
	ClearBox();
	CreateBox();
}

void ASplineBlockingVolume::ClearBox()
{
	// 모든 자식 컴포넌트를 찾아서 제거 (기존 BoxComponents 배열과 관계없이)
	TArray<USceneComponent*> ChildComponents;
	GetRootComponent()->GetChildrenComponents(true, ChildComponents);
    
	for (USceneComponent* Child : ChildComponents)
	{
		UBoxComponent* BoxComp = Cast<UBoxComponent>(Child);
		if (BoxComp && Child != SplineComponent)
		{
			BoxComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			BoxComp->UnregisterComponent();
			BoxComp->DestroyComponent();
		}
	}
    
	// BoxComponents 배열도 비움
	BoxComponents.Empty();
}

void ASplineBlockingVolume::CreateBox()
{
	Modify();
	
	float SplineLength = SplineComponent->GetSplineLength();
	float Distance = 0.f;
	
	while (Distance < SplineLength)
	{
		float Curvature = CalculateCurvatureAtDistance(Distance);

		float Spacing = FMath::Lerp(MaxSpacing, MinSpacing, FMath::Clamp(Curvature / CurvatureMax, 0.f, 1.f));

		FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
		FRotator Rotation = SplineComponent->GetRotationAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);

		// Box 컴포넌트 생성
		UBoxComponent* NewBox = NewObject<UBoxComponent>(this);
		NewBox->SetFlags(RF_Transactional);
		NewBox->Modify();
		NewBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		NewBox->RegisterComponent();

		NewBox->SetCollisionObjectType(ECC_WorldStatic);
		
		NewBox->SetBoxExtent(CollisionBoxExtent);
		NewBox->SetWorldLocation(Location);
		NewBox->SetWorldRotation(Rotation);

		NewBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		NewBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

		BoxComponents.Add(NewBox);

		// 다음 생성 위치로 이동
		Distance += Spacing;
	}
}

float ASplineBlockingVolume::CalculateCurvatureAtDistance(float Distance) const
{
	const float Delta = 1.f;

	FVector Tangent1 = SplineComponent->GetTangentAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
	FVector Tangent2 = SplineComponent->GetTangentAtDistanceAlongSpline(FMath::Min(Distance + Delta, SplineComponent->GetSplineLength()), ESplineCoordinateSpace::World);

	FVector Derivative = (Tangent2 - Tangent1) / Delta;

	float Speed = Tangent1.Size();
	if (Speed < KINDA_SMALL_NUMBER) return 0.f;

	float Curvature = Derivative.Size() / FMath::Pow(Speed, 3);
	return Curvature;
}
