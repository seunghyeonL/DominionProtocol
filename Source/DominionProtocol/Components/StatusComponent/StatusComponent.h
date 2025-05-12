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

class UStatusEffectBase;
struct FStatusComponentInitializeData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusComponent();

	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnStaminaChanged OnStaminaChanged;

protected:
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAIStateBase> AIState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, float> StatMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, float> StatMultiplierMap;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, UStatusEffectBase*> StatusEffectMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	FGameplayTagContainer ActiveStatusEffectTags;

private:
	//CheatManager
	bool bIsInfiniteStaminaMode = false;

public:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	FORCEINLINE FGameplayTagContainer& GetActiveStatusEffectTags() { return ActiveStatusEffectTags; }

	FORCEINLINE UAIStateBase* GetAIState() { return AIState; }
	FORCEINLINE void SetAIState(UAIStateBase* InAIState) { AIState = InAIState; }

	UFUNCTION(BlueprintCallable, Category = "Effects")
	float GetStat(const FGameplayTag& StatTag) const;

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void SetStat(const FGameplayTag& StatTag, float Value);
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void SetHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool HasEnoughStamina(float RequiredAmount) const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ConsumeStamina(float Amount);
	
	void InitializeStatusComponent(const FStatusComponentInitializeData& InitializeData);

	void RemoveActiveStatusEffect(FGameplayTag StatusEffectTag);
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	virtual void ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude);
	UFUNCTION(BlueprintCallable, Category = "Effects")
	virtual void ActivateStatusEffectWithDuration(const FGameplayTag& StatusEffectTag, const float Magnitude, float Duration);
	UFUNCTION(BlueprintCallable, Category = "Effects")
	virtual void DeactivateStatusEffect(const FGameplayTag& StatusEffectTag);

	//CheatManager
	FORCEINLINE void SwitchInfiniteStaminaMode() { bIsInfiniteStaminaMode = !bIsInfiniteStaminaMode; }
};
