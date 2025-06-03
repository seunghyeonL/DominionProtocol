// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BaseEnemy.h"
#include "MinionEnemy.generated.h"

class UMnhTracerComponent;
class UMnhBoxComponent;

UCLASS()
class DOMINIONPROTOCOL_API AMinionEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	AMinionEnemy();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMnhBoxComponent> LeftWeaponTraceBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMnhBoxComponent> RightWeaponTraceBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMnhTracerComponent> AttackTraceComponent;
};