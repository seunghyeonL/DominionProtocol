// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControlStateBase.h"
#include "PlayerControlState.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerControlState : public UPlayerControlStateBase, public FTickableGameObject
{
	GENERATED_BODY()

public:
	UPlayerControlState();

	bool bDashTickActive;
	
	virtual void Move(const FInputActionValue& Value) override;
	virtual void Look(const FInputActionValue& Value) override;
	virtual void Dash() override;
	virtual void Sprint() override;
	virtual void Parry() override;
	virtual void BaseAttack() override;
	virtual void WeaponSkill() override;
	virtual void MagicSkill() override;
	virtual void Interact() override;
	virtual void LockOn() override;
	
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override{return bDashTickActive;}
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UPlayerControlState, STATGROUP_Tickables); }
};
