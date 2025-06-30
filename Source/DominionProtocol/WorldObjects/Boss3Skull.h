// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Interface/StoryDependentInterface.h"
#include "Boss3Skull.generated.h"

class UNiagaraSystem;
class ADomiCharacter;
class USphereComponent;

UCLASS()
class DOMINIONPROTOCOL_API ABoss3Skull : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ABoss3Skull();

	void SetIsInteractable(bool bNewIsInteractable);
	
	void CheckStoryStateAndToggleVisibility();
	
// Getter
	FORCEINLINE bool GetIsInBattleRoom() const { return bIsInBattleRoom; }
	FORCEINLINE UStaticMeshComponent* GetSkullMeshComponent() const { return SkullMeshComponent; }
	FORCEINLINE UStaticMeshComponent* GetAltarMeshComponent() const { return AltarMeshComponent; }

// Setter
	FORCEINLINE void SetIsInBattleRoom(bool bNewIsInBattleRoom) { bIsInBattleRoom = bNewIsInBattleRoom; }
	
protected:
	virtual void BeginPlay() override;

	//Interact Interface function
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;
	
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

private:
	UFUNCTION()
	void OnKnockbackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void OnShake();

	void SpawnBoss3();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneRootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SkullMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> AltarMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> InteractableCollisionSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> KnockbackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag BossTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BossClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ShakeTimeDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxShakeTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraSystems;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAudioComponent> ExplosionAudioComponent1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundWave> ExplosionSound1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAudioComponent> ExplosionAudioComponent2;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundWave> ExplosionSound2;
	
private:
	UPROPERTY()
	TObjectPtr<ADomiCharacter> CachedCharacter;

	EGameStoryState StoryStateCache;

	bool bIsInBattleRoom;
	
	FRotator BaseRotation;
	
	float ShakeStartTime;
	
	float TimeCount;

	FTimerHandle ShakeTimerHandle;
};
