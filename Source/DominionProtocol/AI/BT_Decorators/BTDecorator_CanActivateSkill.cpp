// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/BTDecorator_CanActivateSkill.h"
#include "AI/AIControllers/Boss1AIController.h"

UBTDecorator_CanActivateSkill::UBTDecorator_CanActivateSkill()
{
	NodeName = TEXT("Can Using Skill");

	bNotifyTick = false;
}

bool UBTDecorator_CanActivateSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (!CheckSkillTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTDecorator_CanActivateSkill] Failed: No CheckSkillTag"));
		return false;
	}

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTDecorator_CanActivateSkill] Failed: No AIController"));
		return false;
	}

	ABoss1AIController* Boss1AIController = Cast<ABoss1AIController>(AIController);

	return Boss1AIController->CanAttack(CheckSkillTag);
}
