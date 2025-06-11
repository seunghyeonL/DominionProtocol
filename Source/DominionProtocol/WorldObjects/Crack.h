// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Crack.generated.h"

class UWorldInstanceSubsystem;
class ABaseGameMode;
class ATargetPoint;
class ADomiCharacter;
class UCapsuleComponent;
class UDialogueManager;

DECLARE_LOG_CATEGORY_EXTERN(LogCrackSystem, Log, All);

UCLASS()
class DOMINIONPROTOCOL_API ACrack : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

#pragma region Functions
	
public:
	ACrack();
	
	//Getter
	FORCEINLINE const FText& GetCrackName() const { return CrackName; }
	FORCEINLINE bool GetIsActivate() const { return bIsActivate; }
	FVector GetRespawnTargetPointLocation() const;
	FRotator GetRespawnTargetPointRotation() const;

	FORCEINLINE int32 GetCrackIndex() const { return CrackIndex; }
	
	//Setter
	FORCEINLINE void SetActive() { bIsActivate = true; }
	FORCEINLINE void SetCrackName(const FText& NewName) {CrackName = NewName; }
	
protected:
	virtual void BeginPlay() override;
	
	//Interact
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractMessage_Implementation() const override;

private:
	void CheckPlayerInActivationRange();
	void CheckPlayerInInteractionRange();
	void SetDetailDetection(bool bEnable);

#pragma endregion

	
	
#pragma region Variables

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bBlockInteract;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> SceneComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> CapsuleComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TObjectPtr<UChildActorComponent> RespawnTargetPointComp;


	// Audio Start
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAudioComponent> BGMAudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundWave> BGMSound;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundWave> ActivateSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundAttenuation> SoundAttenuation;
	// Audio End
	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FText CrackName; // UI전용

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 CrackIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float InteractableRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ActivationDistanceCalculateRadius;

private:
	UPROPERTY()
	TObjectPtr<ADomiCharacter> CachedCharacter;

	UPROPERTY()
	TObjectPtr<ABaseGameMode> BaseGameMode;

	UPROPERTY()
	TObjectPtr<UWorldInstanceSubsystem> WorldInstanceSubsystem;

	UPROPERTY()
	UDialogueManager* DialogueManager;
	
	UPROPERTY()
	TObjectPtr<ATargetPoint> RespawnTargetPoint;
	
	float DistanceCalculateRadiusSquared;
	
	float InteractableRadiusSquared;
	
	float Distance;
	
	bool bIsActivate;
	
	FTimerHandle DetectionTimerHandle;
	FTimerHandle DetailDetectionTimerHandle;
	
	
#pragma endregion
};
