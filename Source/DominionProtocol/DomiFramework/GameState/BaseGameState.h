// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/SkillData.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

struct FStatusComponentInitializeData;
struct FSkillComponentInitializeData;
class UDomiGameInstance;
class USoundInstanceSubsystem;

UCLASS()
class DOMINIONPROTOCOL_API ABaseGameState : public AGameState
{
	GENERATED_BODY()

//Functions
public:
	ABaseGameState();

	FSkillData* GetSkillData(const FGameplayTag SkillTag) const;
	FSkillComponentInitializeData* GetSkillComponentInitializeData(const FGameplayTag PawnTag) const;
	FStatusComponentInitializeData* GetStatusComponentInitializeData(const FGameplayTag PawnTag) const;
	
protected:
	virtual void BeginPlay() override;

private:
	void InitializeGameInstance();
	void InitializeSoundSubsystem();

//Variables
private:
	UPROPERTY()
	UDomiGameInstance* GameInstance;
	
	UPROPERTY()
	USoundInstanceSubsystem* SoundSubsystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|Skill", meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|Skill|Initialize", meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillInitializeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|Status|Intialize ", meta = (AllowPrivateAccess = "true"))
	UDataTable* StatusInitializeDataTable;
};
