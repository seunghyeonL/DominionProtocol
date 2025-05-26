// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "Util/GameTagList.h"
#include "AIStateComponent.generated.h"

class UAIStateBase;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API UAIStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAIStateComponent();

	// 상태 변경
	void SetAIStateByTag(const FGameplayTag& StateTag);

	// 현재 상태 태그 반환
	FGameplayTag GetCurrentStateTag() const;

	// 델리게이트: 외부에서 상태 변경 알림 받고 싶을 때 바인딩
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAIStateChanged, FGameplayTag);
	FOnAIStateChanged OnAIStateChanged;

protected:
	UPROPERTY()
	TObjectPtr<UAIStateBase> CurrentState;

	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UAIStateBase>> StateMap;
};
