// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BaseEnemy.h"
#include "ProtoNormalEnemy.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API AProtoNormalEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AProtoNormalEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnGroggy() override;
	virtual void OnDeath_Implementation() override;
};
