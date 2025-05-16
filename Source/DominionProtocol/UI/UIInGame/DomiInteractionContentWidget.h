// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiInteractionContentWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiInteractionContentWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetCurrentInteractableActor(AActor* Actor) {  CurrentInteractableActor = Actor; }


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentFocusIndex;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> CurrentInteractableActor;
};
