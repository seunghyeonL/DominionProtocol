// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/InteractableInterface.h"
#include "DyingHelper.generated.h"

class AItemDropped;
class UDialogueManager;

UCLASS()
class DOMINIONPROTOCOL_API ADyingHelper : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	ADyingHelper();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

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
	void Die();

	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> IdleMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TObjectPtr<UAnimMontage> DieMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AItemDropped> ItemDroppedClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> Hair;

	UPROPERTY()
	TObjectPtr<UDialogueManager> DialogueManager;

private:
	FTimerHandle DieTimerHandle;
	
};
