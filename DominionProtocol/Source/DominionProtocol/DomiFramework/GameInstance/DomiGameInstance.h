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
	//=========
	//보스 태그 이름 정해지면 bool 말고 태그 이용한 방식으로 변경해야함
	UPROPERTY()
	TArray<bool> IsBossDeadArray;
	//========

	static const int32 NumBosses;
};
