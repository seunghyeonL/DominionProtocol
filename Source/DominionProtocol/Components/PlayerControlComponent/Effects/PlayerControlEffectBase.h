// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../States/PlayerControlStateBase.h"
#include "Util/GameTagList.h"
#include "PlayerControlEffectBase.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerControlEffectBase : public UPlayerControlStateBase
{
	GENERATED_BODY()

protected:
	FTimerHandle DurationTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	FGameplayTag ControlEffectTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerControlStateBase> InnerState;

public:
	virtual void Activate();
	virtual void Activate(float Duration);
	virtual void Deactivate();
	virtual void Tick(float DeltaTime);

	// Input Binding Functions
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Dash() override;
	virtual void Sprint() override;
	virtual void Parry() override;
	virtual void BaseAttack() override;
	virtual void WeaponSkill() override;
	virtual void MagicSkill() override;
	virtual void Interact() override;
	
	FORCEINLINE UPlayerControlStateBase* GetInnerState() const { return InnerState; }
	FORCEINLINE void SetInnerState(UPlayerControlStateBase* NewInnerState) { InnerState = NewInnerState; }
	
};
