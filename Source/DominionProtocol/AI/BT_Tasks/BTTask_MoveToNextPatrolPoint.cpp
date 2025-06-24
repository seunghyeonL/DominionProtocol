#include "AI/BT_Tasks/BTTask_MoveToNextPatrolPoint.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "GameFramework/Actor.h"

UBTTask_MoveToNextPatrolPoint::UBTTask_MoveToNextPatrolPoint()
{
	NodeName = TEXT("Move To Current Patrol Point");

	BlackboardKey.SelectedKeyName = "CurrentPatrol";

	// 필요 시 세부 설정 (예: 수용 오차)
	AcceptableRadius = 50.f;
	bObserveBlackboardValue = true;
	bAllowStrafe = true;
}