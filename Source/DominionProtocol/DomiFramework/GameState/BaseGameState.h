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
class ACrack;

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
	FORCEINLINE FSkillComponentInitializeData* GetSkillComponentInitializeData(const FGameplayTag PawnTag) const;
	FORCEINLINE FStatusComponentInitializeData* GetStatusComponentInitializeData(const FGameplayTag PawnTag) const;
	FORCEINLINE ACrack* GetCrackByIndex(int32 InCrackIndex) const {return AllCracksCache[InCrackIndex]; }
	
	void CacheAllCracks();

	void InitializeCrackDataMap();

	void AddAnotherLevelFirstCrack();

	ACrack* FindNearestCrack();
	
protected:
	virtual void BeginPlay() override;

private:
	void InitializeGameInstance();
	void InitializeSoundSubsystem();

//Variables
private:
	UPROPERTY()
	UWorld* World;
	
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

	UPROPERTY()
	TArray<ACrack*> AllCracksCache;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
