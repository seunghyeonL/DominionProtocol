// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../States/PlayerControlStateBase.h"
#include "EnumAndStruct/EffectData/EffectUIData.h"
#include "Util/GameTagList.h"
#include "PlayerControlEffectBase.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UPlayerControlEffectBase : public UPlayerControlStateBase
{
	GENERATED_BODY()


protected:
	FTimerHandle DurationTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	FGameplayTag ControlEffectTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float CachedDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UTexture2D* EffectIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerControlStateBase> InnerState;

public:
	UPlayerControlEffectBase();
	virtual void Initialize();
	
	virtual bool Activate();
	virtual bool Activate(float Duration);
	virtual void Deactivate();
	virtual void Tick(float DeltaTime) override;

	// Input Binding Functions
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
	virtual void SwapWeapon()override;

	FORCEINLINE bool IsActive() const { return bIsActive; }
	FORCEINLINE UPlayerControlStateBase* GetInnerState() const { return InnerState; }
	FORCEINLINE void SetInnerState(UPlayerControlStateBase* NewInnerState) { InnerState = NewInnerState; }
	FEffectUIData GetEffectUIData() const;
};
