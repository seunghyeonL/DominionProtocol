// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "PlayerControlComponent.generated.h"

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
	TMap<FGameplayTag, UPlayerControlEffectBase*> ControlEffectMapper;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE UPlayerControlStateBase* GetPlayerControlState() const { return PlayerControlState; };
	FORCEINLINE void SetPlayerControlState(UPlayerControlStateBase* NewState) { PlayerControlState = NewState; };

	UFUNCTION(BlueprintCallable, Category = "Effects")
	FORCEINLINE FGameplayTagContainer& GetActiveControlEffects() { return ActiveControlEffectTags; }

	virtual void ActivateControlEffect(const FGameplayTag& ControlEffectTag);
	virtual void ActivateControlEffectWithDuration(const FGameplayTag& ControlEffectTag, float Duration);
	virtual void DeactivateControlEffect(const FGameplayTag& ControlEffectTag);
	
	// Input Binding Functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Dash();
	void Sprint();
	void Parry();
	void BaseAttack();
	void WeaponSkill();
	void MagicSkill();
	void Interact();
};
