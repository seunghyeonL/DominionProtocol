// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "AIStatusComponent.generated.h"

class UAIStateBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOMINIONPROTOCOL_API UAIStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIStatusComponent();
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIStateBase> CurrentState;

	UPROPERTY(VisibleAnywhere, Category = "AI State")
	FGameplayTag CurrentStateTag;

public:
	void SetAIState(UAIStateBase* NewState);
	UAIStateBase* GetCurrentState() const { return CurrentState; }

	UFUNCTION()
	void SetAIStateByTag(const FGameplayTag& NewTag);

	FGameplayTag GetCurrentStateTag() const;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAIStateChanged, FGameplayTag);
	FOnAIStateChanged OnAIStateChanged;
};
