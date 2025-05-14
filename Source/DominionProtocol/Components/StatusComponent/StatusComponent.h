// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "StatusComponent.generated.h"

class UAIStateBase;
//UI Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, Stamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShieldChanged, float, Shield);

DECLARE_DELEGATE(FOnDeath);
DECLARE_DELEGATE(FOnGroggy);

class UStatusEffectBase;
struct FStatusComponentInitializeData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusComponent();

	FTimerHandle StaminaRecoveryDelayTimer;
	
	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnStaminaChanged OnStaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnShieldChanged OnShieldChanged;

	FOnDeath OnDeath;
	FOnGroggy OnGroggy;

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAIStateBase> AIState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, float> StatMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, float> StatMultiplierMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, UStatusEffectBase*> StatusEffectMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats", meta=(AllowPrivateAccess=true))
	FGameplayTagContainer ActiveStatusEffectTags;
	
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRecoveryRate = 10.f;

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRecoveryDelay = 1.f;

	bool bIsRecoveringStamina = false;
	
	//CheatManager
	bool bIsInfiniteStaminaMode = false;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE FGameplayTagContainer& GetActiveStatusEffectTags() { return ActiveStatusEffectTags; }

	FORCEINLINE UAIStateBase* GetAIState() { return AIState; }
	FORCEINLINE void SetAIState(UAIStateBase* InAIState) { AIState = InAIState; }
	
	float GetStat(const FGameplayTag& StatTag) const;
	void SetStat(const FGameplayTag& StatTag, float Value);
	
	void SetHealth(const float NewHealth);
	void SetShield(const float NewShield);
	void SetStamina(float NewHealth);
	
	bool HasEnoughStamina(float RequiredAmount) const;
	void ConsumeStamina(float Amount);
	void StartStaminaRecovery();
	void StopStaminaRecovery();
	void BlockStaminaRecovery();
	
	void InitializeStatusComponent(const FStatusComponentInitializeData& InitializeData);
	void RemoveActiveStatusEffect(const FGameplayTag StatusEffectTag);
	
	virtual void ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude);
	virtual void ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude, float Duration);
	virtual void DeactivateStatusEffect(const FGameplayTag& StatusEffectTag);

	//CheatManager
	FORCEINLINE void SwitchInfiniteStaminaMode() { bIsInfiniteStaminaMode = !bIsInfiniteStaminaMode; }
};
