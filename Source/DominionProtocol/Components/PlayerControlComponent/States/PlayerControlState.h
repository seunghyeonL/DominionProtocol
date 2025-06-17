// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControlStateBase.h"
#include "PlayerControlState.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerControlState : public UPlayerControlStateBase
{
	GENERATED_BODY()

public:
	UPlayerControlState();
	
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Dash() override;
	virtual void SprintStart() override;
	virtual void SprintEnd() override;
	virtual void Parry() override;
	virtual void BaseAttack() override;
	virtual void WeaponSkill() override;
	virtual void MagicSkill() override;
	virtual void Interact() override;
	virtual void LockOn() override;
	virtual void ConsumeItemAction_1()override;
	virtual void ConsumeItemAction_2()override;
	virtual void ConsumeItemAction_3()override;
	virtual void SwapWeapon() override;
	virtual void InteractionScroll(const FInputActionValue& Value) override;

	virtual void DashAttack() override;

	virtual void Tick(float DeltaTime) override;
	// virtual bool IsTickable() const override{return bDashTickActive;}
	// virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UPlayerControlState, STATGROUP_Tickables); }

	
};
