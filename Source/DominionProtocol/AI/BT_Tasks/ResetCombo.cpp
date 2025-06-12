// Fill out your copyright notice in the Description page of Project Settings.


#include "ResetCombo.h"

#include "AIController.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/SkillComponentUser.h"

UResetCombo::UResetCombo()
{
	NodeName = "ResetCombo";
}

EBTNodeResult::Type UResetCombo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}
	ACharacter* Character = AIController->GetCharacter();
	if (!Character)
	{
		return EBTNodeResult::Failed;
	}
	
	ISkillComponentUser* SkillComponentUser = Cast<ISkillComponentUser>(Character);
	if (!SkillComponentUser)
	{
		return EBTNodeResult::Failed;
	}

	SkillComponentUser->GetSkillComponent()->ResetCombo(SkillGroupTag);
	return EBTNodeResult::Succeeded;
}
