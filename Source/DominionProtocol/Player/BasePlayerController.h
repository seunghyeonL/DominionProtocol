// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class UInputAction;
class UEnhancedInputLocalPlayerSubsystem;
class UFadeWidget;
class UInputMappingContext;

UCLASS(Abstract)
class DOMINIONPROTOCOL_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();

	UFUNCTION()
	void FadeIn(const float PlayTime = -1.f);

	UFUNCTION()
	void FadeOut(const float PlayTime = -1.f);

protected:
	virtual void BeginPlay() override;

	virtual void CreateAndAddHUDWidget();

	virtual void SetupInputMode();

	virtual void SetupMappingContext();

	virtual void BindInputActions();
	
	void RemoveAllMappingContext();

	// 함수 사용 시 GET_FUNCTION_NAME_CHECKED 를 사용하여 FunctionName 을 입력해주세요 
	void HelperBindInputAction(UEnhancedInputComponent* EnhancedInputComponent, const UInputAction* Action, const ETriggerEvent Event, const FName FunctionName);


protected:
	UPROPERTY()
	TArray<TObjectPtr<UInputMappingContext>> MappingContextArray;
	
	UPROPERTY()
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> LocalPlayerInputSubsystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UFadeWidget> FadeWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<UFadeWidget> FadeWidgetInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FadeDuration = 1.f;
};
