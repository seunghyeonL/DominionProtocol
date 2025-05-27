// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Services/BTService_SelectSkill.h"
#include "AI/AIControllers/Boss1AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SelectSkill::UBTService_SelectSkill()
{
	NodeName = TEXT("Calculate Skill to SelfActor");

	bNotifyTick = false;
	bNotifyBecomeRelevant = true;
}

void UBTService_SelectSkill::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AAIController* AIController = OwnerComp.GetAIOwner();

	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTService_SelectSkill] Failed: No AIController"));
		return;
	}

	ABoss1AIController* Boss1AIController = Cast<ABoss1AIController>(AIController);

	if (!Boss1AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTService_SelectSkill] Failed: No Boss1AIController"));
		return;
	}

	FGameplayTag GameplayTag = Boss1AIController->GetAttack();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Warning, TEXT("[UBTService_SelectSkill] Failed: No Blackboard"));
		return;
	}

	// SkillTagName 출력
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,
		FString::Printf(TEXT("SkillTagName: %s"), *GameplayTag.GetTagName().ToString()));

	Blackboard->SetValueAsName("SkillTag", GameplayTag.GetTagName());
}
