// Fill out your copyright notice in the Description page of Project Settings.
UE_DISABLE_OPTIMIZATION

#include "Components/SkillComponent/Skills/SkillObject/LaserActor.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/DecalComponent.h"
#include "Materials/MaterialInterface.h"
#include "Util/DebugHelper.h"

// Sets default values
ALaserActor::ALaserActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OwnerSkillTag = SkillTags::LaserSkill;

	// Trace Setting
	TraceComponent = CreateDefaultSubobject<UMnhTracerComponent>(TEXT("TraceComponent"));

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);

	CapsuleComponent = CreateDefaultSubobject<UMnhCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(SceneComponent);
	CapsuleComponent->SetCapsuleHalfHeight(750.f);
	CapsuleComponent->SetRelativeLocation(FVector(750.f, 0.f, 0.f));
	CapsuleComponent->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	FMnhTracerConfig TracerConfig;

	TracerConfig.TracerTag = OwnerSkillTag;
	TracerConfig.DrawDebugType = EDrawDebugTrace::ForOneFrame;
	TracerConfig.TraceSettings.TraceChannel = ECC_Pawn;
	TracerConfig.TraceSource = EMnhTraceSource::MnhShapeComponent;

	TraceComponent->TracerConfigs.Add(TracerConfig);
	TraceComponent->FilterType = EMnhFilterType::None;

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(OwnerSkillTag);

	TraceComponent->InitializeTracers(TagContainer, CapsuleComponent);

	LaserBeamEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserBeamEffect"));
	LaserBeamEffect->SetupAttachment(CapsuleComponent);

	LaserFireBallEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserFireBallEffect"));
	LaserFireBallEffect->SetupAttachment(CapsuleComponent);

	LaserHitEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserHitEffect"));
	LaserHitEffect->SetupAttachment(CapsuleComponent);
}

void ALaserActor::SetOwnerCharacter(AActor* NewOwnerCharacter)
{
	OwnerCharacter = NewOwnerCharacter;
}

void ALaserActor::HandleTracerHit(FGameplayTag TracerTag, FHitResult HitResult, float DeltaTime)
{
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))
	{
		return;
	}

	UStatusComponent* StatusComponent = HitActor->FindComponentByClass<UStatusComponent>();

	if (!IsValid(StatusComponent))
	{
		return;
	}

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		return;
	}

	// 충돌 파라미터 설정
	FCollisionQueryParams Params;

	// 소유 캐릭터 무시
	Params.AddIgnoredActor(OwnerCharacter);
	Params.AddIgnoredActor(this);

	const FVector Start = GetActorLocation();
	const FVector End = HitResult.ImpactPoint;

	FHitResult HitResult2;

	bool bHit = World->LineTraceSingleByChannel(
		HitResult2,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (IsValid(HitResult2.GetActor()) && HitResult2.GetActor() != HitResult.GetActor())
	{
		AActor* Temp = HitResult2.GetActor();
		return;
	}

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	if (IsValid(SkillComponent))
	{
		UBaseSkill* BaseSkill = SkillComponent->GetCurrentSkill();

		if (IsValid(BaseSkill))
		{
			if (BaseSkill->GetSkillTag() == SkillTags::LaserSkill)
			{
				BaseSkill->ApplyAttackToHitActor(HitResult, DeltaTime);
			}
		}
	}
}

// Called when the game starts or when spawned
void ALaserActor::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(
		InitDelayTimer,
		this,
		&ThisClass::Init,
		1.f,
		false
	);

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

		if (IsValid(BaseGameState))
		{
			if (FSkillData* SkillData = BaseGameState->GetSkillData(OwnerSkillTag))
			{
				if (SkillData->NaiagaraParticle.Num() >= 3)
				{
					LaserFireBallEffect = NewObject<UNiagaraComponent>(this);
					LaserFireBallEffect->SetAsset(SkillData->NaiagaraParticle[1]);
					LaserFireBallEffect->RegisterComponent();
					LaserFireBallEffect->AttachToComponent(
						RootComponent,
						FAttachmentTransformRules::SnapToTargetIncludingScale
					);
				}
			}
		}
	}
}

void ALaserActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 타이머 해제
	if (InitDelayTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(InitDelayTimer);
	}
}

void ALaserActor::Init()
{
	if (TraceComponent)
	{
		// 델리게이트 바인딩
		TraceComponent->OnHitDetected.AddDynamic(this, &ThisClass::HandleTracerHit);
	}

	FGameplayTagContainer TagContainer;
	TagContainer.AddTag(OwnerSkillTag);

	TraceComponent->AddToIgnoredActors(TagContainer, OwnerCharacter);
	TraceComponent->StartTracers(TagContainer);

	UWorld* World = GetWorld();

	if (IsValid(World))
	{
		ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();

		if (IsValid(BaseGameState))
		{
			if (FSkillData* SkillData = BaseGameState->GetSkillData(OwnerSkillTag))
			{
				if (SkillData->NaiagaraParticle.Num() >= 3)
				{
					LaserBeamEffect = NewObject<UNiagaraComponent>(this);
					LaserBeamEffect->SetAsset(SkillData->NaiagaraParticle[0]);
					LaserBeamEffect->RegisterComponent();
					LaserBeamEffect->AttachToComponent(
						RootComponent,
						FAttachmentTransformRules::SnapToTargetIncludingScale
					);

					LaserHitEffect = NewObject<UNiagaraComponent>(this);
					LaserHitEffect->SetAsset(SkillData->NaiagaraParticle[2]);
					LaserHitEffect->RegisterComponent();
					LaserHitEffect->AttachToComponent(
						RootComponent,
						FAttachmentTransformRules::SnapToTargetIncludingScale
					);
				}

				if (SkillData->SkillMaterials.Num() >= 2)
				{
					DecalMaterial1 = SkillData->SkillMaterials[0];
					DecalMaterial2 = SkillData->SkillMaterials[1];
				}
			}
		}
	}
}

// Called every frame
void ALaserActor::Tick(float DeltaTime)
{
	if (!IsValid(OwnerCharacter))
	{
		return;
	}

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	if (IsValid(SkillComponent))
	{
		UBaseSkill* BaseSkill = SkillComponent->GetCurrentSkill();

		if (BaseSkill == nullptr || BaseSkill->GetSkillTag() != SkillTags::LaserSkill)
		{
			Destroy();
		}
	}

	Super::Tick(DeltaTime);

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		return;
	}

	// 충돌 파라미터 설정
	FCollisionQueryParams Params;

	// 소유 캐릭터 무시
	Params.AddIgnoredActor(OwnerCharacter);

	const FVector Start = GetActorLocation();
	const FVector End = Start + GetActorForwardVector() * 1500.f;

	FHitResult HitResult;

	bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	//// 디버그 라인 그리기
	//DrawDebugLine(
	//	World,
	//	Start,
	//	End,
	//	bHit ? FColor::Green : FColor::Red,
	//	false,
	//	2.f,
	//	0,
	//	2.0f
	//);

	const float Distance = bHit ? HitResult.Distance : 1500.f;

	check(LaserBeamEffect);
	check(LaserHitEffect);

	LaserBeamEffect->SetVariablePosition(FName("Start"), Start);
	LaserBeamEffect->SetVariablePosition(FName("End"), Start + GetActorForwardVector() * Distance);

	if (bHit)
	{
		LaserHitEffect->SetVisibility(true);
		LaserHitEffect->SetWorldLocation(HitResult.Location);
		LaserHitEffect->SetWorldRotation(UKismetMathLibrary::MakeRotFromX(HitResult.ImpactNormal));

		FVector DecalSize;

		DecalSize.X = FMath::FRandRange(5.f, 20.f);
		DecalSize.Y = FMath::FRandRange(5.f, 20.f);
		DecalSize.Z = FMath::FRandRange(5.f, 20.f);

		// 데칼 생성
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			DecalMaterial1,
			DecalSize,
			HitResult.Location,
			FRotator(0.0f, 0.0f, -90.0f),
			2.f
		);

		DecalSize.X = FMath::FRandRange(10.f, 40.f);
		DecalSize.Y = FMath::FRandRange(10.f, 40.f);
		DecalSize.Z = FMath::FRandRange(10.f, 40.f);

		// 데칼 생성
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),
			DecalMaterial2,
			DecalSize,
			HitResult.Location,
			FRotator(0.0f, 0.0f, -90.0f),
			4.f
		);
	}
	else
	{
		LaserHitEffect->SetVisibility(false);
	}

	CapsuleComponent->SetCapsuleHalfHeight(Distance / 2);
}

