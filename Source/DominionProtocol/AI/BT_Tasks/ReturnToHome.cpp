// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Tasks/ReturnToHome.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Components/AIComponent/AIStateComponent.h"
#include "Navigation/PathFollowingComponent.h"

UReturnToHome::UReturnToHome()
{
	NodeName = TEXT("Return To Home");
	bNotifyTick = true;

	HomeLocationKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UReturnToHome, HomeLocationKey));
}

EBTNodeResult::Type UReturnToHome::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	ControlledActor = AICon->GetPawn();
	if (!ControlledActor) return EBTNodeResult::Failed;

	FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(HomeLocationKey.SelectedKeyName);
	TargetLocation = HomeLocation;

	AICon->MoveToLocation(TargetLocation, AcceptableRadius);
	bMoving = true;

	FAIMoveRequest MoveRequest(TargetLocation);
	MoveRequest.SetAcceptanceRadius(AcceptableRadius);
	FNavPathSharedPtr NavPath;
	FPathFollowingRequestResult Result = AICon->MoveTo(MoveRequest, &NavPath);

	UE_LOG(LogTemp, Warning, TEXT("MoveTo Result Code: %d"), static_cast<int32>(Result.Code));

	return EBTNodeResult::InProgress;
}


void UReturnToHome::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!ControlledActor) return;

	const float Distance = FVector::Dist(ControlledActor->GetActorLocation(), TargetLocation);

	if (Distance <= AcceptableRadius + 50)
	{
		if (UAIStateComponent* AIStateComp = ControlledActor->FindComponentByClass<UAIStateComponent>())
		{
			AIStateComp->SetAIStateByTag(EffectTags::Idle);
			UE_LOG(LogTemp, Warning, TEXT("Returned Home → Set to Idle"));
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
