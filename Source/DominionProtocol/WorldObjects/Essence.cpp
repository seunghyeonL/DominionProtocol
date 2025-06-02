// Fill out your copyright notice in the Description page of Project Settings.


#include "Essence.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "DomiFramework/GameInstance/DomiGameInstance.h"
#include "Player/Characters/DomiCharacter.h"

#include "Util/DebugHelper.h"

AEssence::AEssence()
	:	MaxSpeed(5000.f),
		InitialSpeed(0.f),
		Accelerate(300),
		EssenceAmount(1),
		CurrentSpeed(InitialSpeed)
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	NaigaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NaigaComponent");
	NaigaraComponent->SetupAttachment(SceneRoot);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(SceneRoot);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AEssence::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEssence::OnOverlapBegin);

	PlayerCharacter = Cast<ADomiCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	checkf(PlayerCharacter, TEXT("AEssence::BeginPlay : DomiCharacter Not Found"));
}

void AEssence::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(PlayerCharacter))
	{
		Debug::PrintError(TEXT("AEssence::Tick : DomiCharacter is Invalid"));
		return;
	}
	
	FVector NewPlayerLocation = PlayerCharacter->GetActorLocation();
	FVector NewDirection = (NewPlayerLocation - GetActorLocation()).GetSafeNormal();
	
	CurrentSpeed += Accelerate * DeltaTime;
	CurrentSpeed = FMath::Clamp(CurrentSpeed, 0.f, MaxSpeed);

	FVector Movement = NewDirection * CurrentSpeed * DeltaTime;
	AddActorWorldOffset(Movement);
}

void AEssence::OnOverlapBegin(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor) && !OtherActor->ActorHasTag("Player"))
	{
		return;
	}

	if (OtherActor == PlayerCharacter)
	{
		if (EssenceAmount != 0)
		{
			UDomiGameInstance* GameInstance = GetGameInstance<UDomiGameInstance>();
			if (IsValid(GameInstance))
			{
				GameInstance->AddPlayerCurrentEssence(EssenceAmount);
			}
			Destroy();
		}
		else
		{
			Debug::PrintError(TEXT("AEssence::OnHitWithPlayer : EssenceAmount Set is missing"));
		}
	}
}