// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "EnumAndStruct/FItemSubsystemData.h"
#include "ItemInstanceSubsystem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UItemInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region Share
	
public:

	//Save & Load
	void LoadSaveData(const FItemSubsystemData& SaveData);
	FItemSubsystemData GetSaveData() const;
	
protected:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

private:

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
