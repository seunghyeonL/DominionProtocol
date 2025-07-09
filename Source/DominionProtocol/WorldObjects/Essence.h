// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Essence.generated.h"

class ADomiCharacter;
class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraComponent;

UCLASS()
class DOMINIONPROTOCOL_API AEssence : public AActor
{
	GENERATED_BODY()

public:
	AEssence();

	void SetEssenceAmount(int32 InEssenceAmount) { EssenceAmount = InEssenceAmount; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<UNiagaraComponent> NaigaraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Property")
	float MaxSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Property")
	float InitialSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Property")
	int32 Accelerate;
	
private:
	UPROPERTY()
	TObjectPtr<ADomiCharacter> PlayerCharacter;
	
	int32 EssenceAmount;

	float CurrentSpeed;
};
