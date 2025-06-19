// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewSaveSlot.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UNewSaveSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void StartGame(const int32 SlotIndex) const;
	
	UFUNCTION(BlueprintCallable)
	void LoadGame(const int32 SlotIndex) const;
	
	UFUNCTION(BlueprintCallable)
	void DeleteGame(const int32 SlotIndex) const;

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class USaveManagerSubsystem> SaveManagerSubsystemInstance;

};
