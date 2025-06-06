// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/EffectData/EffectUIData.h"
#include "UObject/Object.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "StatusEffectBase.generated.h"


UCLASS(BlueprintType)
class DOMINIONPROTOCOL_API UStatusEffectBase : public UObject
{
	GENERATED_BODY()

protected:
	UStatusEffectBase();
	
	FTimerHandle DurationTimer;
	FTimerHandle DoTTimer;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Owner", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ACharacter> OwnerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	FGameplayTag StatusEffectTag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float CachedDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	float DurationRemained;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UTexture2D* EffectIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
	float Magnitude;

public:
	virtual void Initialize();
	
	FORCEINLINE void SetOwnerCharacter(ACharacter* InOwnerCharacter) { OwnerCharacter = InOwnerCharacter; }
	FORCEINLINE void SetMagnitude(float InMagnitude) { Magnitude = InMagnitude; }
	FEffectUIData GetEffectUIData() const;

	UFUNCTION(BlueprintCallable)
	bool IsActive() const { return bIsActive; }

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetStatusEffectTag() const { return StatusEffectTag; }
	
	virtual bool Activate();
	virtual bool Activate(float Duration);
	virtual void Deactivate();
	virtual void Tick(float DeltaTime);
};
