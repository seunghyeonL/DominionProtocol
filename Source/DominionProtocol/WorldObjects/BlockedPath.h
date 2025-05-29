// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumAndStruct/EGameStoryState.h"
#include "Interface/StoryDependentInterface.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h" 
#include "BlockedPath.generated.h"

class UBoxComponent;

UCLASS()
class DOMINIONPROTOCOL_API ABlockedPath : public AActor, public IStoryDependentInterface
{
	GENERATED_BODY()
	
public:	
	ABlockedPath();

	virtual void OnStoryStateUpdated_Implementation(EGameStoryState NewState) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void TryOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void OnOpened();

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere)
	UNiagaraComponent* PathEffect;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* NiagaraTemplate;

	UPROPERTY(EditAnywhere, Category = "BlockedPath")
	bool bIsBlocking;

	UPROPERTY(EditAnywhere, Category = "BlockedPath")
	EGameStoryState RequiredStoryState = EGameStoryState::Tutorial;
};
