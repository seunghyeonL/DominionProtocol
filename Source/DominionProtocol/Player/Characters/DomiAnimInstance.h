// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Util/GameTagList.h"
#include "DomiAnimInstance.generated.h"

class UCharacterMovementComponent;
class ADomiCharacter;

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UDomiAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UDomiAnimInstance();

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(visibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<ACharacter> Character;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "References")
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActiveEffects")
	FGameplayTagContainer ActiveAnimEffects;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MovementData")
	bool bIsPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MovementData")
	FVector Velocity;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MovementData")
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MovementData")
	bool bShouldMove;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "MovementData")
	bool bIsFalling;
	
};
