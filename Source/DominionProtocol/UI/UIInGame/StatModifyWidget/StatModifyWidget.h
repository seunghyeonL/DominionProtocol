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
public:
	UFUNCTION(BlueprintCallable)
	void UpdatePreviewStat();

	UFUNCTION(BlueprintCallable)
	void InitializeWidgetDatas();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateStatModifyData();
	
protected:
	virtual void NativeConstruct() override;

	// ButtonActionFunctions
	UFUNCTION(BlueprintCallable)
	void OnStrUpButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnStrDownButtonClicked();
	
	UFUNCTION(BlueprintCallable)
	void OnLifeUpButtonClicked();
	
	UFUNCTION(BlueprintCallable)
	void OnLifeDownButtonClicked();
	
	UFUNCTION(BlueprintCallable)
	void OnSplUpButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnSplDownButtonClicked();
	
	UFUNCTION(BlueprintCallable)
	void OnEndUpButtonClicked();

	UFUNCTION(BlueprintCallable)
	void OnEndDownButtonClicked();
	
	UFUNCTION(BlueprintCallable)
	void OnDecideButtonClicked();

protected:
	bool bCanLevelUp;

	float AccumulatedRequiredEssence;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStatusComponent> PlayerStatusComponent;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGameplayTag> PlayerStatTags;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, float> PlayerStatPreviewData;
	
	TMap<FGameplayTag, int32> StatModifiedNumMap;

	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, FString> StatModifiedStringMap;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FGameplayTag, FSlateColor> ActivatedStatModifyMap;

	UPROPERTY(BlueprintReadOnly)
	FSlateColor ActivatedLevelUpRequiredTextColor;
	
	UPROPERTY(BlueprintReadOnly)
	float CurrentEssenceValue;

	UPROPERTY(BlueprintReadOnly)
	float LevelUpRequiredEssence;

};
