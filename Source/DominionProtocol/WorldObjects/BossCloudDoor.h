#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/InteractableInterface.h"
#include "Interface/StoryDependentInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h" 
#include "BossCloudDoor.generated.h"

class UBoxComponent;
class ADomiCharacter;

UCLASS()
class DOMINIONPROTOCOL_API ABossCloudDoor : public AActor, public IInteractableInterface, public IStoryDependentInterface
{
	GENERATED_BODY()

public:
	ABossCloudDoor();

	virtual void OnStoryStateUpdated_Implementation(EGameStoryState NewState) override;
protected:
	void BeginPlay() override;
	void EnterDoor();
	void OnEnterMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	virtual void Interact_Implementation(AActor* Interactor) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FText GetInteractMessage_Implementation() const;
	
private:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BlockingBox;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* PathEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraTemplate;

	UPROPERTY()
	ADomiCharacter* CachedCharacter;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* EnterDoorMontage;

protected:
	bool bIsMontagePlaying;

	UPROPERTY(EditAnywhere, Category = "BlockedPath")
	EGameStoryState RequiredStoryState;
};
