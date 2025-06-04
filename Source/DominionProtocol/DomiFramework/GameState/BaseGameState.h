// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/SkillData.h"
#include "EnumAndStruct/FCrackInitializeData.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

struct FEffectInitializeData;
struct FCrackInitializeData;
class UWorldInstanceSubsystem;
struct FStatusComponentInitializeData;
struct FSkillComponentInitializeData;
class UDomiGameInstance;
class USoundInstanceSubsystem;
class ACrack;
class ALightWeightActor;

UCLASS()
class DOMINIONPROTOCOL_API ABaseGameState : public AGameState
{
	GENERATED_BODY()

#pragma region Share
	
//Functions
public:
	ABaseGameState();
	
	//Getter
	FORCEINLINE FSkillData* GetSkillData(const FGameplayTag SkillTag) const;
	FSkillComponentInitializeData* GetSkillComponentInitializeData(const FGameplayTag PawnTag) const;
	FStatusComponentInitializeData* GetStatusComponentInitializeData(const FGameplayTag PawnTag) const;
	FEffectInitializeData* GetEffectInitializeData(const FGameplayTag EffectTag) const;
	FORCEINLINE ACrack* GetCrackByIndex(int32 InCrackIndex) const {return AllCracksCache[InCrackIndex]; }
	
	void CacheAllCracks();
	
	void LoadCrackDataFromInstance();

	void InitializeCrackDataMap();

	ACrack* FindNearestCrack();
	
protected:
	virtual void BeginPlay() override;

private:
	//Initialize Instances
	void InitializeGameInstance();
	void InitializeWorldInstanceSubsystem();
	void InitializeSoundSubsystem();
	void InitializeZeroIndexCrackData(const FString CurrentLevelName);
	

//Variables
private:
	UPROPERTY()
	UWorld* World;
	
	UPROPERTY()
	UDomiGameInstance* GameInstance;

	UPROPERTY()
	UWorldInstanceSubsystem* WorldInstanceSubsystem;
	
	UPROPERTY()
	USoundInstanceSubsystem* SoundSubsystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|Crack", meta = (AllowPrivateAccess = "true"))
	UDataTable* CrackInitializeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|Skill", meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|Skill|Initialize", meta = (AllowPrivateAccess = "true"))
	UDataTable* SkillInitializeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|Status|Intialize ", meta = (AllowPrivateAccess = "true"))
	UDataTable* StatusInitializeDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|Status|Intialize ", meta = (AllowPrivateAccess = "true"))
	UDataTable* EffectInitializeDataTable;
	
	UPROPERTY()
	TArray<ACrack*> AllCracksCache;

	bool bIsNewGame = true;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
