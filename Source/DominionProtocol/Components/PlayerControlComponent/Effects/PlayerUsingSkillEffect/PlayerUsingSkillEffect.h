// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/PlayerControlComponent/Effects/PlayerControlEffectBase.h"
#include "PlayerUsingSkillEffect.generated.h"

class UBaseBufferedInput;
/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UPlayerUsingSkillEffect : public UPlayerControlEffectBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	TArray<UBaseBufferedInput*> BufferedInputArray;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPlayerControlComponent> ControlComponent;
	
	FGameplayTagContainer DoubleExecuteSkillEffectTags;

public:
	UPlayerUsingSkillEffect();

	virtual void Initialize() override;

	FORCEINLINE void SetControlEffectTag(FGameplayTag InControlEffectTag) { ControlEffectTag = InControlEffectTag; }
	FORCEINLINE void SetControlComponent(UPlayerControlComponent* InControlComponent) { ControlComponent = InControlComponent; }
	FORCEINLINE bool IsDoubleExecuteSkillEffect() const { return DoubleExecuteSkillEffectTags.HasTagExact(ControlEffectTag); }
	
	virtual bool Activate() override;
	virtual bool Activate(float Duration) override;
	virtual void Deactivate() override;
	
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
	virtual void ConsumeItemAction_1() override;
	virtual void ConsumeItemAction_2() override;
	virtual void ConsumeItemAction_3() override;

	virtual void Tick(float DeltaTime) override;
};
