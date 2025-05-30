// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BaseEnemy.h"
#include "BaseBossEnemy.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDeath);

UCLASS()
class DOMINIONPROTOCOL_API ABaseBossEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	ABaseBossEnemy();

	UPROPERTY(BlueprintAssignable)
	FOnBossDeath OnBossDeathDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	int32 BossID;

public:
	FORCEINLINE int32 GetBossID() { return BossID; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void OnDeath() override;
};