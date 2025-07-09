// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AlertWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UAlertWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnShowPlayerDeathAlert();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowPlayerDeathAlert();

	UFUNCTION()
	void OnShowBossKillAlert(AActor* DeadMonster);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowBossKillAlert(const AActor* DeadMonster);

	UFUNCTION()
	void OnShowEssenceRestoredAlert(const int32 NewEssenceAmount);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowEssenceRestoredAlert(const int32 NewEssenceAmount);

	UFUNCTION()
	void OnShowCrackActivationAlert(const FText CrackName);

	UFUNCTION(BlueprintImplementableEvent)
	void ShowCrackActivationAlert(const FText& CrackName);
	
protected:
	virtual void NativeConstruct() override;

	void BindAlertDelegates();
	void BindRestoredEssence(class ADropEssence* DropEssence);


public:
	FDelegateHandle HandleBindRestoredEssence;


protected:
	UPROPERTY(BlueprintReadOnly)
	FString BossMonsterName;
};
