// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/BTDecorator_CheckSkillTag.h"
#include "BTDecorator_CheckSkillTag.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckSkillTag::UBTDecorator_CheckSkillTag()
{
	NodeName = TEXT("Check SkillTag");

	bNotifyTick = false;
}

bool UBTDecorator_CheckSkillTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (!CheckSkillTag.IsValid())
	{
		return false;
	}

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	FName SkillTagName = Blackboard->GetValueAsName("SkillTag");
	FName CheckSkillTagName = CheckSkillTag.GetTagName();

	return SkillTagName == CheckSkillTagName;
}
