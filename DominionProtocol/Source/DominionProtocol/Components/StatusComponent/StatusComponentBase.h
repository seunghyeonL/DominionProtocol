// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "StatusComponentBase.generated.h"


class UStatusEffectBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API UStatusComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UStatusComponentBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, float> BattleStats;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, float> BattleStatMultipliers;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, TSubclassOf<UStatusEffectBase>> StatusEffectClasses;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, UStatusEffectBase*> ActiveStatusEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	FGameplayTagContainer ActiveStatusEffectTags;

public:
	UFUNCTION(BlueprintCallable, Category = "Effects")
	FORCEINLINE FGameplayTagContainer& GetActiveStatusEffectTags() { return ActiveStatusEffectTags; }

	UFUNCTION(BlueprintCallable, Category = "Effects")
	virtual float GetStat(FGameplayTag StatTag) const;
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void SetHealth(float NewHealth);
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	virtual void ActivateStatusEffect(const FGameplayTag& StatusEffectTag, const float Magnitude);
	UFUNCTION(BlueprintCallable, Category = "Effects")
	virtual void ActivateStatusEffectWithDuration(const FGameplayTag& StatusEffectTag, const float Magnitude, float Duration);
	UFUNCTION(BlueprintCallable, Category = "Effects")
	virtual void DeactivateStatusEffect(const FGameplayTag& StatusEffectTag);
};
