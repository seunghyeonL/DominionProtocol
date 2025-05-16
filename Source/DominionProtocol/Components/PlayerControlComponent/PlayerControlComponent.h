// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "PlayerControlComponent.generated.h"

class UBaseBufferedInput;
class UPlayerControlStateBase;
class UPlayerControlEffectBase;
struct FInputActionValue;

// DECLARE_DELEGATE(FOnComponentReady);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API UPlayerControlComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerControlComponent();
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FORCEINLINE void SetValidBufferedInput(UBaseBufferedInput* InBufferedInput) { ValidBufferedInput = InBufferedInput; }
	
	// FOnComponentReady OnComponentReady;
	// bool bIsComponentReady;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerControlStateBase> PlayerControlState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	FGameplayTagContainer ActiveControlEffectTags;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	TMap<FGameplayTag, UPlayerControlEffectBase*> ControlEffectMap;

	UPROPERTY()
	TObjectPtr<UBaseBufferedInput> ValidBufferedInput;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UPlayerControlStateBase* GetPlayerControlState() const { return PlayerControlState; };
	FORCEINLINE void SetPlayerControlState(UPlayerControlStateBase* NewState) { PlayerControlState = NewState; };

	// UFUNCTION(BlueprintCallable, Category = "Effects")
	FORCEINLINE FGameplayTagContainer& GetActiveControlEffectTags() { return ActiveControlEffectTags; }

	virtual void ActivateControlEffect(const FGameplayTag& ControlEffectTag);
	virtual void ActivateControlEffect(const FGameplayTag& ControlEffectTag, float Duration);
	virtual void DeactivateControlEffect(const FGameplayTag& ControlEffectTag);
	
	// Input Binding Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dash();
	void SprintStart();
	void SprintEnd();
	void Parry();
	void BaseAttack();
	void WeaponSkill();
	void MagicSkill();
	void Interact();
	void LockOn();
	void ConsumeItemAction_1();
	void ConsumeItemAction_2();
	void ConsumeItemAction_3();
	void SwapWeapon();
};
