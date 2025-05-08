// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DomiCharacter3DWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiCharacter3DWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetOwningActor(AActor* NewOwner);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;
};
