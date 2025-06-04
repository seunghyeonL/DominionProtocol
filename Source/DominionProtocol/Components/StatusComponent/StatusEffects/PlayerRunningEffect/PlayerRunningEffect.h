// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusEffects/StatusEffectBase.h"
#include "PlayerRunningEffect.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UPlayerRunningEffect : public UStatusEffectBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coefficient")
	float SpeedCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Coefficient")
	float StaminaPerSecond;

public:
	UPlayerRunningEffect();

	virtual bool Activate() override;
	virtual bool Activate(float Duration) override;
	virtual void Deactivate() override;

	virtual void Tick(float DeltaTime) override;
};
