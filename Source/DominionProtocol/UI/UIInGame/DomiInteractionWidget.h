// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiInteractionWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UDomiInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void BindInteractionDelegates();

	UFUNCTION()
	void UpdateInteractableActorArray(TArray<AActor*> NewInteractableActorArray);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateInteractionWidget();
	
	virtual void NativeConstruct() override;


protected:

	UPROPERTY()
	TArray<UUserWidget*> InteractionList; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AActor*> InteractableActorArray;
	
	UPROPERTY()
	TObjectPtr<class UDomiWidgetComponent> WidgetComponent;
};
