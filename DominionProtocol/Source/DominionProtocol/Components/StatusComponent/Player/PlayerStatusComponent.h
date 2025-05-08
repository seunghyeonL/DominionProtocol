// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusComponentBase.h"
#include "PlayerStatusComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DOMINIONPROTOCOL_API UPlayerStatusComponent : public UStatusComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	TMap<FGameplayTag, float> BaseStats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats", meta=(AllowPrivateAccess=true))
	float Stamina;
	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual float GetStat(FGameplayTag StatTag) const override;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool HasEnoughStamina(float RequiredAmount) const;

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ConsumeStamina(float Amount);

};
