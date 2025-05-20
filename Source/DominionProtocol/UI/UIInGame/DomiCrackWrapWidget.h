// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnumAndStruct/FCrackData.h"
#include "DomiCrackWrapWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiCrackWrapWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<FCrackData> Level1CrackArray;

	UPROPERTY(BlueprintReadWrite)
	TArray<FCrackData> Level2CrackArray;

	UPROPERTY(BlueprintReadWrite)
	bool ActivateCrackButton = false;
};
