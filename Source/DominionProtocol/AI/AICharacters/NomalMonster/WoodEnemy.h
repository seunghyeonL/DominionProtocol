// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/NomalMonster/NomalEnemy.h"
#include "WoodEnemy.generated.h"

class UMnhTracerComponent;
class UMnhBoxComponent;

UCLASS()
class DOMINIONPROTOCOL_API AWoodEnemy : public ANomalEnemy
{
	GENERATED_BODY()
public:
	AWoodEnemy();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void Onewitch();
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMnhBoxComponent> WeaponTraceBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMnhTracerComponent> AttackTraceComponent;
};
