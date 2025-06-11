// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Util/GameTagList.h"
#include "StatModifyWidget.generated.h"

class UStatusComponent;
class UTextBlock;
class UButton;


UCLASS()
class DOMINIONPROTOCOL_API UStatModifyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	bool bCanLevelUp;

	float AccumulatedRequiredEssence;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStatusComponent> PlayerStatusComponent;

	TArray<FGameplayTag> PlayerStatTags;
	TMap<FGameplayTag, float> PlayerStatPreviewData;
	TMap<FGameplayTag, int32> StatModifiedNumMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, UTextBlock*> TextBlockMap;

	void InitializeWidgetDatas();
	
	UFUNCTION()
	void UpdatePreviewStat();

	// ButtonBindingFunctions
	UFUNCTION()
	void OnStrUpButtonClicked();

	UFUNCTION()
	void OnStrDownButtonClicked();
	
	UFUNCTION()
	void OnLifeUpButtonClicked();
	
	UFUNCTION()
	void OnLifeDownButtonClicked();
	
	UFUNCTION()
	void OnSplUpButtonClicked();

	UFUNCTION()
	void OnSplDownButtonClicked();
	
	UFUNCTION()
	void OnEndUpButtonClicked();

	UFUNCTION()
	void OnEndDownButtonClicked();
	
	UFUNCTION()
	void OnDecideButtonClicked();
	
	// Stat TextBlocks
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StrTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LifeTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SplTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EndTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PrimaryAttackPowerTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SubAttackPowerTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MagicPowerTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxHealthTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MaxStaminaTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelTextBlock;

	// Essence TextBlocks
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CurrentEssenceTextBlock;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelUpRequiredTextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AccumulatedEssenceTextBlock;

	// Buttons
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StrUpButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StrDownButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LifeUpButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> LifeDownButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SplUpButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SplDownButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EndUpButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> EndDownButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DecideButton;
	
};
