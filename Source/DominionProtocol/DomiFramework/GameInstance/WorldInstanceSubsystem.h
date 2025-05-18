// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "WorldInstanceSubsystem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UWorldInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Share
	
public:

protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;


private:
	UPROPERTY()
	TSet<FName> WorldNames;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
