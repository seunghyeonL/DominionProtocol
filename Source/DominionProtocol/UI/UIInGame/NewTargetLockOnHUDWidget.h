// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewTargetLockOnHUDWidget.generated.h"


UCLASS()
class DOMINIONPROTOCOL_API UNewTargetLockOnHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateLockOnTargetActor(AActor* NewTarget);
	
	UFUNCTION()
	void ChangeActivateLockOn(bool bIsActive);

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySearchingLockOnTargetAnim();

protected:
	virtual void NativeConstruct() override;

	void BindLockOnTargetActorDelegate();

	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> CurrentTargetActor;

	UPROPERTY()
	TObjectPtr<class UWidgetComponent> LockOnWidgetComponent;

	UPROPERTY(BlueprintReadOnly)
	bool bIsActiveLockOn = false;
};
