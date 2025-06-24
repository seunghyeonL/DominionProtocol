// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/NomalEnemy.h"
#include "AI/AIUtil/PatrolPoint.h"
#include "AI/AIControllers/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h" 

ANomalEnemy::ANomalEnemy()
{
    PrimaryActorTick.bCanEverTick = true;

    //bUseControllerRotationYaw = false;

    //GetCharacterMovement()->bOrientRotationToMovement = true;
    //GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
}

void ANomalEnemy::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

    if (auto* MeshComp = GetMesh())
    {
        MeshComp->SetSimulatePhysics(true);  // 물리 시뮬레이션 시작
        MeshComp->SetCollisionProfileName("Ragdoll");  // 적절한 콜리전 프로필 설정
        MeshComp->bPauseAnims = true;        // 애니메이션 중지
        MeshComp->bNoSkeletonUpdate = false;
    }
	SetLifeSpan(5.f);
}

void ANomalEnemy::BeginPlay()
{
	Super::BeginPlay();


    if (bUsePatrolPoint)
    {
        float MaxSearchRadius = 2000.f;

        TArray<AActor*> PatrolCandidates;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), APatrolPoint::StaticClass(), PatrolCandidates);

        PatrolCandidates = PatrolCandidates.FilterByPredicate([this, MaxSearchRadius](AActor* Actor)
            {
                return FVector::DistSquared(this->GetActorLocation(), Actor->GetActorLocation()) <= FMath::Square(MaxSearchRadius);
            });

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
                UBlackboardComponent* BB = AIController->GetBlackboardComponent();
                BB->SetValueAsObject("PatrolPointA", Closest);
                BB->SetValueAsObject("PatrolPointB", SecondClosest);
                BB->SetValueAsBool("bShouldPatrol", true);
                BB->SetValueAsVector("CurrentPatrol", Closest->GetActorLocation());
            }
        }
    }
}