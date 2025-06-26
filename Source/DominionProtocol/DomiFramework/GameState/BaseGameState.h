// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/SkillData.h"
#include "EnumAndStruct/FCrackInitializeData.h"
#include "EnumAndStruct/FCrackImageData.h"
#include "GameFramework/GameState.h"
#include "BaseGameState.generated.h"

struct FPhysicalSurfaceTypeData;
class ADropEssence;
class USaveManagerSubsystem;
class ABaseGameMode;
class UItemInstanceSubsystem;
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
	UFUNCTION(BlueprintCallable)
	const TMap<int32, FCrackImageData>& GetPastCrackImageDataMap();
	UFUNCTION(BlueprintCallable)
	const TMap<int32, FCrackImageData>& GetPresentCrackImageData();
	
	FSkillData* GetSkillData(const FGameplayTag SkillTag) const;
	FSkillComponentInitializeData* GetSkillComponentInitializeData(const FGameplayTag PawnTag) const;
	FStatusComponentInitializeData* GetStatusComponentInitializeData(const FGameplayTag PawnTag) const;
	FEffectInitializeData* GetEffectInitializeData(const FGameplayTag EffectTag) const;
	
	FORCEINLINE ACrack* GetCrackByIndex(int32 InCrackIndex) const {return AllCracksCache[InCrackIndex]; }
	FName GetSurfaceNameByEnum(EPhysicalSurface PhysicalSurfaceType) const;
	FPhysicalSurfaceTypeData* GetPhysicalSurfaceTypeData(EPhysicalSurface PhysicalSurfaceType) const;
	FORCEINLINE USoundMix* GetVolumeControlMix() const
	{
		if (VolumeControlMix)
		{
			return VolumeControlMix;	
		}
		return nullptr;
	}
	FORCEINLINE USoundClass* GetMasterClass() const
	{
		if (MasterClass)
		{
			return MasterClass;	
		}
		return nullptr;
	}
	FORCEINLINE USoundClass* GetSFXClass() const
	{
		if (SFXClass)
		{
			return SFXClass;	
		}
		return nullptr;
	}
	FORCEINLINE USoundClass* GetBGMClass() const
	{
		if (BGMClass)
		{
			return BGMClass;	
		}
		return nullptr;
	}
	FORCEINLINE USoundClass* GetUIClass() const
	{
		if (UIClass)
		{
			return UIClass;	
		}
		return nullptr;
	}

	void InitializeGame();
	
protected:
	virtual void BeginPlay() override;

	//Initialize Instances
	void InitializeGameInstance();
	void InitializeSaveManagerInstanceSubsystem();
	void InitializeWorldInstanceSubsystem();
	void InitializeSoundSubsystem();
	void InitializeItemInstanceSubsystem();
	void InitializeZeroIndexCrackData(const FString CurrentLevelName);

	//=====Call in BaseGameMode::StartPlay
	void CacheAllCracks();
	
	void LoadCrackDataFromInstance();

	void LoadItemDataFromInstance();

	void InitializeCrackDataMap();

	ACrack* FindNearestCrack();
	//=====
	
	//Load DataTable Data
	void LoadCrackImageData();
	
//Variables
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<ADropEssence> DropEssenceClass;
	
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|PhysicalSurface ", meta = (AllowPrivateAccess = "true"))
	UDataTable* SurfaceDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|CrackImage")
	UDataTable* PastCrackImageData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DataTable|CrackImage")
	UDataTable* PresentCrackImageData;
	
	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	TObjectPtr<ABaseGameMode> BaseGameMode;
	
	UPROPERTY()
	TObjectPtr<UDomiGameInstance> GameInstance;

	UPROPERTY()
	TObjectPtr<USaveManagerSubsystem> SaveManagerSubsystem;

	UPROPERTY()
	TObjectPtr<UWorldInstanceSubsystem> WorldInstanceSubsystem;
	
	UPROPERTY()
	TObjectPtr<USoundInstanceSubsystem> SoundSubsystem;

	UPROPERTY()
	TObjectPtr<UItemInstanceSubsystem> ItemInstanceSubsystem;
	
	UPROPERTY()
	TArray<ACrack*> AllCracksCache;

	UPROPERTY(EditDefaultsOnly)
	USoundMix* VolumeControlMix;

	UPROPERTY(EditDefaultsOnly)
	USoundClass* MasterClass;

	UPROPERTY(EditDefaultsOnly)
	USoundClass* SFXClass;

	UPROPERTY(EditDefaultsOnly)
	USoundClass* BGMClass;

	UPROPERTY(EditDefaultsOnly)
	USoundClass* UIClass;

	TMap<int32, FCrackImageData> PastCrackImageDataMap;
	TMap<int32, FCrackImageData> PresentCrackImageDataMap;

#pragma endregion


#pragma region KyuHyeok

#pragma endregion


#pragma region SeoYoung

#pragma endregion
};
