// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Util/GameTagList.h"
#include "BaseAIController.generated.h"

class UAISenseConfig_Sight;
class UAIStateComponent;
struct FAIStimulus;
class UBTTaskNode;

UCLASS()
class DOMINIONPROTOCOL_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseAIController();

	// Set target which is closest
	UFUNCTION()
	void EvaluateTargetPriority();

	void SetAIState(const FGameplayTag& NewState);

	void SetCachedTask(UBTTaskNode* NewCachedTask);

	void ClearCachedTask();

	virtual FGameplayTag GetAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	//타겟 감지
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	void HandleTargetLost();

	FTimerHandle LoseTargetTimerHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIStateComponent* AIStateComponent;

	UPROPERTY()
	UBTTaskNode* CachedTask;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
	float AISightRadius = 500;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
	float LoseSightRadius = 600;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Perception")
	float AIVisionAngle = 360;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool IsUsingSkill() const;
	bool HasTarget() const;
	class UAIStateComponent* GetAIStateComponent() const;
	void EvaluateTargetPerception();
	void NotifyNearbyAllies(AActor* SensedActor);
};
