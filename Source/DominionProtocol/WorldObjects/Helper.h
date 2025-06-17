#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "Helper.generated.h"

class UAnimMontage;
class UDialogueManager;

DECLARE_DELEGATE(FOnHelperAppearFinished);
DECLARE_DELEGATE(FOnHelperDisappearFinished);

UCLASS()
class DOMINIONPROTOCOL_API AHelper : public ACharacter
{
	GENERATED_BODY()
	
public:	
	AHelper();

	virtual void Tick(float DeltaTime) override;

	void Appear(const FVector& SpawnLocation, const FRotator& SpawnRotation);
	void Disappear();

	void SetDialogueManager(UDialogueManager* InManager);
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDisappearMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnAppearMontageEnded(UAnimMontage* Montage, bool bInterrupted);

public:
	FOnHelperAppearFinished OnAppearFinishedCallback;
	FOnHelperDisappearFinished OnDisappearFinishedCallback;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AppearMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DisappearMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	UNiagaraSystem* AppearEffect;

	UPROPERTY()
	UDialogueManager* DialogueManager;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Hair;


};
