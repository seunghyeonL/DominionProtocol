#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss5SpawnEnemyAttack.generated.h"

class UBlackboardComponent;

UENUM()
enum class EColorType : uint8
{
	Red,
	Green,
	Yellow,
	Black,
	Blue,
	None
};

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

private:
	void InitializeArray();

	void SpawnCapsule();

	void SpawnEnemy();

	void DestroySpawnedBoss();

	void End();

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedCapsule;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedNormalEnemy;

	UPROPERTY()
	TObjectPtr<AActor> SpawnedBossEnemy;

	TObjectPtr<UBlackboardComponent> BBComp;

	EColorType CurrentColorType;
	FVector TargetLocation;

	float ElapsedTime = 0.0f;
	float Duration = 2.0f;

	TArray<FVector> CapsuleStartLocation;
	TArray<FVector> CapsuleEndLocation;

	TArray<FVector> SpawnedCapsuleLocation;
	TArray<FVector> SpawnedEnemyLocation;
	TArray<FRotator> SpawnedEnemyRotation;

	int32 RandomIndex;

	FTimerHandle SpawnedBossAILifeSpanTimer;
};