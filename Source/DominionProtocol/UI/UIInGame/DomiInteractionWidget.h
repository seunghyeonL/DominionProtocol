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
	void UpdateInteractableActorSet(TSet<AActor*> NewInteractableActorSet);

	UFUNCTION()
	void UpdateInteractableWidgetFocusing(const float ScrollValue);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateInteractionWidget();

	UFUNCTION(blueprintImplementableEvent, BlueprintCallable)
	void ChangeWidgetFocus(const float InputValue);
	
	virtual void NativeConstruct() override;


protected:

	UPROPERTY()
	TSet<UUserWidget*> InteractionList; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TSet<AActor*> InteractableActorSet;
	
	UPROPERTY()
	TObjectPtr<class UDomiWidgetComponent> WidgetComponent;
};
