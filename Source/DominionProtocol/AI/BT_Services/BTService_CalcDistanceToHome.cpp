// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Services/BTService_CalcDistanceToHome.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"

UBTService_CalcDistanceToHome::UBTService_CalcDistanceToHome()
{
	NodeName = TEXT("Check Distance to Home");

	// 서비스 실행 간격 설정
	Interval = 1.0f;
	RandomDeviation = 0.0f;
}

void UBTService_CalcDistanceToHome::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto* Blackboard = OwnerComp.GetBlackboardComponent();
	APawn* ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (!Blackboard || !ControlledPawn) return;

	const FVector Home = Blackboard->GetValueAsVector("HomeLocation");
	const float Distance = FVector::Dist(ControlledPawn->GetActorLocation(), Home);
	UObject* Target = Blackboard->GetValueAsObject("TargetActor");

	if (!Target && Distance > 1000.f)
	{
		//Blackboard->SetValueAsName("CurrentState", "Return");
	}
}