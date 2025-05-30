// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/NomalMonster/SkeletonEnemy.h"
#include "KnifeSkeletonEnemy.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API AKnifeSkeletonEnemy : public ASkeletonEnemy
{
	GENERATED_BODY()
public:
	AKnifeSkeletonEnemy();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TemporarilyHideWeapon(float HideDuration = 1.5f);

	FTimerHandle HideWeaponTimerHandle;
private:
	void ShowWeapon();
};
