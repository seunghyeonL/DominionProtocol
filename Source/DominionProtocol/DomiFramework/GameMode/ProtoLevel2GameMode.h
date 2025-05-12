// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "ProtoLevel2GameMode.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API AProtoLevel2GameMode : public ABaseGameMode
{
	GENERATED_BODY()

public:
	AProtoLevel2GameMode();
	
protected:
	virtual void BeginPlay() override;
	
private:
};
