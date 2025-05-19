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
	void SetReceivedInteractableActor(AActor* Actor);


protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CurrentWidgetFocusIndex;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> ReceivedInteractableActor;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ADomiCharacter> OwningCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bHasFocus = false;
};
