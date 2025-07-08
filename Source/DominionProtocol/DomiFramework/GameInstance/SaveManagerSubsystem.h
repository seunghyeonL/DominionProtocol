// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumAndStruct/FSaveSlotMetaData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveManagerSubsystem.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API USaveManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//======UI 에서 호출할 함수======
	UFUNCTION(BlueprintCallable)
	void StartNewGame(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void LoadSaveDataAndOpenLevel(int32 SlotIndex);

	UFUNCTION(BlueprintCallable)
	void DeleteSaveSlot(int32 SlotIndex);
	//============================
	
	bool SaveGame(const FString& SlotName, int32 UserIndex = 0);
	bool LoadGame(const FString& SlotName, int32 UserIndex = 0);

	bool SaveSettings();
	bool LoadSettings();

	// Setter
	FORCEINLINE void SetWorldCache(UWorld* InWorld) { World = InWorld;}
	FORCEINLINE void SetSaveSlotData(int32 SlotIndex, const FSaveSlotMetaData& Data) { SaveSlotArray[SlotIndex] = Data; }

	// Getter
	FORCEINLINE int32 GetPlayTime(int32 SlotIndex) { return SaveSlotArray[SlotIndex].PlayTime; }
	TArray<FSaveSlotMetaData> GetSaveSlotArray() { return SaveSlotArray; }
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<FSaveSlotMetaData> SaveSlotArray;

private:
	// TitleLevel에서만 사용됨
	UPROPERTY()
	TObjectPtr<UWorld> World;
};
