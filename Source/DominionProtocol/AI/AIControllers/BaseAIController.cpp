// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/AIComponent/AIState/AIStateBase.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/AIComponent/AIStateComponent.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AI/BT_Tasks/ExecutePattern.h"
#include "Player/Characters/DomiCharacter.h"
#include "Util/DebugHelper.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "AI/AICharacters/BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h" 

// Sets default values
ABaseAIController::ABaseAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Perception 컴포넌트 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));

	// 시야 감각 구성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = AISightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AIVisionAngle;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	// 델리게이트 바인딩
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnTargetPerceptionUpdated);

	AIStateComponent = CreateDefaultSubobject<UAIStateComponent>(TEXT("AIStateComponent"));

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

	if (AIStateComponent)
	{
		AIStateComponent->SetAIStateByTag(EffectTags::Idle);
		SetAISpeed();
	}
}

void ABaseAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	EvaluateTargetPriority();
	EvaluateTargetPerception();
	//if (!AIStateComponent) return;

	//if (Stimulus.WasSuccessfullySensed() && IsValid(Actor))
	//{
	//	if (GetWorld()->GetTimerManager().IsTimerActive(LoseTargetTimerHandle))
	//	{
	//		GetWorld()->GetTimerManager().ClearTimer(LoseTargetTimerHandle);
	//	}
	//	AIStateComponent->SetAIStateByTag(EffectTags::Combat);
	//	APawn* MyPawn = GetPawn();
	//	if (MyPawn)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("[AI] %s"), *MyPawn->GetName());
	//		UE_LOG(LogTemp, Warning, TEXT("[State] Before Change: %s"), *AIStateComponent->GetCurrentStateTag().ToString());
	//	}
	//	SetAISpeed();
	//}
	//else
	//{
	//	if (IsUsingSkill())
	//	{
	//		return;
	//	}
	//	AIStateComponent->SetAIStateByTag(EffectTags::Idle);
	//	APawn* MyPawn = GetPawn();
	//	if (MyPawn)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("[AI] %s"), *MyPawn->GetName());
	//		UE_LOG(LogTemp, Warning, TEXT("[State] Before Change: %s"), *AIStateComponent->GetCurrentStateTag().ToString());
	//	}
	//	SetAISpeed();
	//	GetWorld()->GetTimerManager().SetTimer(LoseTargetTimerHandle, this, &ABaseAIController::HandleTargetLost, 3.0f, false);	
	//}
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

			const ADomiCharacter* Player = Cast<ADomiCharacter>(Target);
			if (!Player) continue;

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

void ABaseAIController::SetCachedTask(UBTTaskNode* NewCachedTask)
{
	CachedTask = NewCachedTask;
}

void ABaseAIController::ClearCachedTask()
{
	UExecutePattern* ExecutePattern = Cast<UExecutePattern>(CachedTask);

	if (IsValid(ExecutePattern))
	{
		ExecutePattern->OnAnimationCompleted();
	}
}

FGameplayTag ABaseAIController::GetAttack()
{
	return FGameplayTag::EmptyTag;
}

// Called every frame
void ABaseAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseAIController::HandleTargetLost()
{
	if (!AIStateComponent) return;

	if (!GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")))
	{
		AIStateComponent->SetAIStateByTag(EffectTags::Return);
		SetAISpeed();
	}
}

bool ABaseAIController::IsUsingSkill() const
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (USkillComponent* SkillComp = ControlledPawn->FindComponentByClass<USkillComponent>())
		{
			return SkillComp->IsUsingSkill();
		}
	}
	return false;
}

bool ABaseAIController::HasTarget() const
{
	if (Blackboard)
	{
		AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
		return IsValid(TargetActor);
	}
	return false;
}

UAIStateComponent* ABaseAIController::GetAIStateComponent() const
{
	return FindComponentByClass<UAIStateComponent>();
}

void ABaseAIController::EvaluateTargetPerception()
{
	if (!AIStateComponent || !Blackboard) return;

	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject("TargetActor"));
	//AActor* TargetActor = Cast<AActor>(GetBlackboardComponent()->GetValueAsObject("TargetActor"));

	if (IsValid(TargetActor))
	{
		AIStateComponent->SetAIStateByTag(EffectTags::Combat);
		SetAISpeed();
	}
	else
	{
		if (IsUsingSkill()) return;
		AIStateComponent->SetAIStateByTag(EffectTags::Idle);
		SetAISpeed();
		GetWorld()->GetTimerManager().SetTimer(LoseTargetTimerHandle, this, &ABaseAIController::HandleTargetLost, 3.0f, false);
	}
}

void ABaseAIController::NotifyNearbyAllies(AActor* SensedActor)
{
	TArray<AActor*> NearbyMonsters;
	float ShareRadius = 1000.f;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseEnemy::StaticClass(), NearbyMonsters);

	for (AActor* MonsterActor : NearbyMonsters)
	{
		if (MonsterActor == GetPawn()) continue;
		if (FVector::Dist(MonsterActor->GetActorLocation(), GetPawn()->GetActorLocation()) > ShareRadius) continue;

		if (AAIController* AllyController = Cast<AAIController>(MonsterActor->GetInstigatorController()))
		{
			UBlackboardComponent* BB = AllyController->GetBlackboardComponent();
			if (BB)
			{
				BB->SetValueAsObject("TargetActor", SensedActor);
			}
		}
	}
}

void ABaseAIController::SetAISpeed()
{
	if (!AIStateComponent) return;

	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(GetPawn());
	if (!BaseEnemy) return;

	const FGameplayTag CurrentState = AIStateComponent->GetCurrentStateTag();

	if (CurrentState == EffectTags::Combat)
	{
		BaseEnemy->GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
	else if (CurrentState == EffectTags::Idle || CurrentState == EffectTags::Return)
	{
		BaseEnemy->GetCharacterMovement()->MaxWalkSpeed = 350.f;
	}
}