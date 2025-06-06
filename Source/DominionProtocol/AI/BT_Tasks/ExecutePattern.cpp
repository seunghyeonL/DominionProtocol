// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecutePattern.h"
#include "AIController.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/SkillComponentUser.h"
#include "Util/DebugHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIControllers/BaseAIController.h"
#include "GameFramework/Character.h"

UExecutePattern::UExecutePattern()
{
	NodeName = TEXT("Execute Pattern");
	bCreateNodeInstance = true;
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

	CachedTemp = &OwnerComp;

	ABaseAIController* BaseAIController = Cast<ABaseAIController>(AIController);

	if (IsValid(BaseAIController))
	{
		BaseAIController->SetCachedTask(this);
	}
	
	SkillComponentUser->ExecuteSkill(SkillGroupTag);

	return EBTNodeResult::InProgress;
}

void UExecutePattern::OnAnimationCompleted()
{
	FinishLatentTask(*CachedTemp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UExecutePattern::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (Character && Character->GetMesh())
	{
		Character->GetMesh()->GetAnimInstance()->Montage_Stop(0.f);
	}

	return EBTNodeResult::Aborted;
}