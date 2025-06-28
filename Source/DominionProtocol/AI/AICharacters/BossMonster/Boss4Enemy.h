// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AICharacters/BossMonster/BaseBossEnemy.h"
#include "Interface/InteractableInterface.h"
#include "WorldObjects/DialogueManager.h"
#include "Boss4Enemy.generated.h"

class UBoxComponent;
class ADomiCharacter;

UCLASS()
class DOMINIONPROTOCOL_API ABoss4Enemy : public ABaseBossEnemy, public IInteractableInterface
{
	GENERATED_BODY()
	
public:
	ABoss4Enemy();

	FORCEINLINE UDialogueManager* GetDialogueManager() { return DialogueManager; }

protected:
	virtual void BeginPlay() override;
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;

	UFUNCTION(BlueprintCallable)
	void DropItem();

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void StartFade();
	void UpdateFade();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

private:
	UPROPERTY()
	ADomiCharacter* CachedCharacter;

	UPROPERTY(EditAnywhere)
	FString DialogueID;

	UPROPERTY()
	TObjectPtr<UDialogueManager> DialogueManager;

	FTimerHandle FadeTimerHandle;

	float FadeElapsedTime = 0.0f; // 현재까지 경과한 시간

	UPROPERTY(EditAnywhere, Category = "Boss Spawn Settings")
	float FadeDuration = 5.0f; // 메테리얼 변화 연출이 걸리는 시간

	UPROPERTY(EditAnywhere, Category = "Boss Spawn Settings")
	UMaterialParameterCollection* FadeMPC;

	UMaterialParameterCollectionInstance* MPCInstance;

	UPROPERTY(EditAnywhere, Category = "Boss Spawn Settings")
	UCurveFloat* FadeCurve;
};
