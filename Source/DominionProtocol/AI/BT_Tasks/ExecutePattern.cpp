// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecutePattern.h"
#include "AIController.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/SkillComponentUser.h"
#include "Util/DebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIControllers/Boss1AIController.h"

UExecutePattern::UExecutePattern()
{
	NodeName = TEXT("Execute Pattern");
}

EBTNodeResult::Type UExecutePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		Debug::PrintError(TEXT("UExecutePattern::ExecuteTask : Invalid AIController."));
		return EBTNodeResult::Failed;
	}

	AActor* OwnerActor = AIController->GetPawn();
	if (!OwnerActor)
	{
		Debug::PrintError(TEXT("UExecutePattern::ExecuteTask : Invalid OwnerActor."));
		return EBTNodeResult::Failed;
	}

	auto SkillComponentUser = Cast<ISkillComponentUser>(OwnerActor);
	if (!SkillComponentUser)
	{
		Debug::PrintError(TEXT("UExecutePattern::ExecuteTask : OwnerActor doesn't implement ISkillComponentUser.."));
		return EBTNodeResult::Failed;
	}

	/*UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	FName SkillTagName = Blackboard->GetValueAsName("SkillTag");*/

	CachedTemp = &OwnerComp;

	FTimerHandle MontageEndTimer;
	//GetWorld()->GetTimerManager().SetTimer(MontageEndTimer, this, &ThisClass::OnAnimationCompleted, 0.5f, false);

	ABoss1AIController* Boss1AIController = Cast<ABoss1AIController>(AIController);

	if (IsValid(Boss1AIController))
	{
		Boss1AIController->SetCachedTask(this);
	}
	
	SkillComponentUser->ExecuteSkill(SkillGroupTag);

	return EBTNodeResult::InProgress;
}

void UExecutePattern::OnAnimationCompleted()
{
	FinishLatentTask(*CachedTemp, EBTNodeResult::Succeeded);
}