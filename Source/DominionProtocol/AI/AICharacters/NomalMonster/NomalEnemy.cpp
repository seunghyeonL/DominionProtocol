// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/NomalEnemy.h"
#include "AI/AIUtil/PatrolPoint.h"
#include "AI/AIControllers/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ANomalEnemy::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

	SetLifeSpan(5.f);
}

void ANomalEnemy::BeginPlay()
{
	Super::BeginPlay();

    if (bUsePatrolPoint)
    {
        TArray<AActor*> PatrolCandidates;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), PatrolCandidates);

        PatrolCandidates.Sort([this](const AActor& A, const AActor& B) {
            return FVector::DistSquared(this->GetActorLocation(), A.GetActorLocation()) <
                FVector::DistSquared(this->GetActorLocation(), B.GetActorLocation());
            });

        if (PatrolCandidates.Num() >= 2)
        {
            APatrolPoint* Closest = Cast<APatrolPoint>(PatrolCandidates[0]);
            APatrolPoint* SecondClosest = Cast<APatrolPoint>(PatrolCandidates[1]);

            AAIController* AIController = Cast<AAIController>(GetController());
            if (AIController && AIController->GetBlackboardComponent())
            {
                AIController->GetBlackboardComponent()->SetValueAsObject("PatrolPointA", Closest);
                AIController->GetBlackboardComponent()->SetValueAsObject("PatrolPointB", SecondClosest);
                AIController->GetBlackboardComponent()->SetValueAsBool("bShouldPatrol", true);
                AIController->GetBlackboardComponent()->SetValueAsObject("CurrentPatrol", Closest);
            }
        }
    }

}
