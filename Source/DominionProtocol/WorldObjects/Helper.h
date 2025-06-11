#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraSystem.h"
#include "Helper.generated.h"

class UAnimMontage;

UCLASS()
class DOMINIONPROTOCOL_API AHelper : public ACharacter
{
	GENERATED_BODY()
	
public:	
	AHelper();

	void Appear(const FVector& SpawnLocation);
	void Disappear();
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDisappearMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnAppearMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* AppearMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DisappearMontage;

};
