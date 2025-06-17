// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI3D/DomiCharacter3DWidget.h"
#include "NewTargetLockOnWidget.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UNewTargetLockOnWidget : public UDomiCharacter3DWidget
{
	GENERATED_BODY()


public:
	UFUNCTION()
	void UpdateLockOnTargetActor(AActor* NewTargetActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnUpdateTargetLockOnActor();

	UFUNCTION()
	void ChangeActivateLockOn(bool bIsActive);

	UFUNCTION(BlueprintImplementableEvent)
	void OnChangeActivateLockOn();

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;

	void SetWidgetRotation();

	virtual void SetOwningActor(AActor* NewOwner) override;

	void SetupTargetLockOnWidget();



protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> CurrentTargetActor;

	UPROPERTY()
	TObjectPtr<class UWidgetComponent> WidgetComponent;

	UPROPERTY(BlueprintReadOnly)
	bool bIsActiveLockOn = false;
};
