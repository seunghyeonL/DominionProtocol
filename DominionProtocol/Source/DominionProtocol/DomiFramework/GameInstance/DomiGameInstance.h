// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DomiGameInstance.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UDomiGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UDomiGameInstance();

	//Setter
	FORCEINLINE void SetIsBossDead(int32 BossIndex);
	//Getter
	FORCEINLINE bool GetIsBossDead(int32 BossIndex);
	
protected:
	
private:
	UPROPERTY()
	TArray<bool> IsBossDeadArray;

	static const int32 NumBosses;
};
