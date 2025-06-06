// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Services/BTService_ChooseRandomPattern.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_ChooseRandomPattern::UBTService_ChooseRandomPattern()
{
	NodeName = "Choose Random Pattern";
	bNotifyBecomeRelevant = true;
}

void UBTService_ChooseRandomPattern::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return;

	int32 RandomValue = FMath::RandRange(0, MaxPatternIndex);
	BBComp->SetValueAsInt(PatternKey.SelectedKeyName, RandomValue);
}
