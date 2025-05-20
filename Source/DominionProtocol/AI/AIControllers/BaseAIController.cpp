// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/StatusComponent/StatusEffects/AIState/AIStateBase.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/StatusComponent/StatusEffects/AIState/AIState_Idle.h"
#include "Components/StatusComponent/StatusEffects/AIState/AIState_Combat.h"
#include "Components/StatusComponent/StatusEffects/AIState/AIState_Return.h"


// Sets default values
ABaseAIController::ABaseAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Perception 컴포넌트 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// 시야 감각 구성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 500.f;
	SightConfig->LoseSightRadius = 500.f;
	SightConfig->PeripheralVisionAngleDegrees = 360.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// 델리게이트 바인딩
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated);
}

// Called when the game starts or when spawned
void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}

	if (InPawn && GetBlackboardComponent())
	{
		const FVector SpawnLocation = InPawn->GetActorLocation();
		GetBlackboardComponent()->SetValueAsVector(TEXT("HomeLocation"), SpawnLocation);
	}

	if (UStatusComponent* StatusComp = InPawn->FindComponentByClass<UStatusComponent>())
	{
		IdleState = NewObject<UAIState_Idle>(StatusComp);
		CombatState = NewObject<UAIState_Combat>(StatusComp);
		ReturnState = NewObject<UAIState_Return>(StatusComp);

		IdleState->Activate();
	}
	UE_LOG(LogTemp, Warning, TEXT("IdleState Activated"));
}

void ABaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	EvaluateTargetPriority();

	if (Stimulus.WasSuccessfullySensed())
	{
		if (CombatState && !CombatState->IsActive())
		{
			CombatState->StateTag = EffectTags::Combat;
			CombatState->Activate();
			UE_LOG(LogTemp, Warning, TEXT("CombatState Activated"));
		}
	}
	else
	{
		IdleState->Activate();
		GetWorld()->GetTimerManager().SetTimer(LoseTargetTimerHandle, this, &ABaseAIController::HandleTargetLost, 3.0f, false);
	}
}

void ABaseAIController::EvaluateTargetPriority()
{
	if (!IsValid(GetPawn())) return;

	TArray<AActor*> PerceivedActors;
	AIPerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);

	AActor* BestTarget = nullptr;
	float ClosestDist = FLT_MAX;

	if (APawn* MyPawn = GetPawn())
	{
		const FVector MyLocation = MyPawn->GetActorLocation();

		for (AActor* Target : PerceivedActors)
		{
			if (!IsValid(Target)) continue;

			const float Dist = FVector::Dist(MyLocation, Target->GetActorLocation());
			if (Dist < ClosestDist)
			{
				BestTarget = Target;
				ClosestDist = Dist;
			}
		}
	}

	UObject* CurrentTarget = GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
	if (!BestTarget && CurrentTarget != BestTarget)
	{
		GetBlackboardComponent()->ClearValue(TEXT("TargetActor"));
	}
	else
	{
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), BestTarget);
	}
}

// Called every frame
void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseAIController::HandleTargetLost()
{
	if (!GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")))
	{
		if (ReturnState && !ReturnState->IsActive())
		{
			ReturnState->Activate();
		}
	}
}