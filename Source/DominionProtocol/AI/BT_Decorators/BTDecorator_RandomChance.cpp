// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BT_Decorators/BTDecorator_RandomChance.h"
#include "BTDecorator_RandomChance.h"

UBTDecorator_RandomChance::UBTDecorator_RandomChance()
{
    NodeName = "Random 50% Chance";
}

bool UBTDecorator_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    // 50% 확률로 true/false 반환
    return FMath::RandBool();
}
