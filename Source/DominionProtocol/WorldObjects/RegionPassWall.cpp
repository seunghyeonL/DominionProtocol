// Fill out your copyright notice in the Description page of Project Settings.


#include "RegionPassWall.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Components/ArrowComponent.h"

#include "Util/DebugHelper.h"

ARegionPassWall::ARegionPassWall()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(SceneComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(SceneComponent);
}

void ARegionPassWall::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ARegionPassWall::OnEndOverlap);
	PreviousRegionIndex = NextRegionIndex - 1;
}

void ARegionPassWall::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor) || !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	FVector PassWallDirection = GetActorForwardVector();
	FVector PlayerDirection = (GetActorLocation() - PlayerCharacter->GetActorLocation()).GetSafeNormal();
	
	float DotProduct = FVector::DotProduct(PassWallDirection, PlayerDirection);

	if (DotProduct > 0)
	{
		OnRegionChanged.Broadcast(NextRegionIndex);
	}
	if (DotProduct < 0)
	{
		OnRegionChanged.Broadcast(PreviousRegionIndex);
	}
}
