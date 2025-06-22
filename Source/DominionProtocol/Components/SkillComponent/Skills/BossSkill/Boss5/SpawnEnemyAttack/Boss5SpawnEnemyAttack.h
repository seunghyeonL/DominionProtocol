#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss5SpawnEnemyAttack.generated.h"

class UBlackboardComponent;

UENUM()
enum class ECapsuleState : uint8
{
	None,
	Rising,
	Holding,
	Falling,
};

ECapsuleState CapsuleState = ECapsuleState::None;

UCLASS()
class DOMINIONPROTOCOL_API UBoss5SpawnEnemyAttack : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UBoss5SpawnEnemyAttack();

	virtual void Tick(float DeltaTime) override;

	virtual void Execute() override;

	void InitializeArray();

	void SpawnCapsule();

	void SpawnEnemy();

	void End();

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedCapsule;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedEnemy;

	TObjectPtr<UBlackboardComponent> BBComp;
	FVector TargetLocation;

	float ElapsedTime = 0.0f;
	float Duration = 2.0f;

	TArray<FVector> CapsuleStartLocation;
	TArray<FVector> CapsuleEndLocation;

	TArray<FVector> SpawnedCapsuleLocation;
	TArray<FVector> SpawnedEnemyLocation;
	TArray<FRotator> SpawnedEnemyRotation;
};