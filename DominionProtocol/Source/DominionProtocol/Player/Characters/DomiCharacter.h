// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Damagable.h"
#include "../EffectReceivable.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "DomiCharacter.generated.h"

class UStatusComponent;
class UPlayerControlComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FAttackData;

//UI Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStaminaChanged, float, Stamina);

UCLASS()
class DOMINIONPROTOCOL_API ADomiCharacter :
public ACharacter, public IDamagable, public IEffectReceivable, public IStatusComponentUser
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerControlComponent> ControlComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatusComponent> StatusComponent;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

public:
	ADomiCharacter();

	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stats|Events")
	FOnStaminaChanged OnStaminaChanged;
	
protected:
	// Bind After ControlComponent Ready
	void BindInputFunctions();
	
	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// StateComponentUser
	virtual void OnDeath() override;
	virtual void OnGroggy() override { /* Not used in player*/ };
	
	// Damagable
	virtual void OnAttacked_Implementation(const FAttackData& AttackData) override;

	// EffectReceivable
	virtual FGameplayTagContainer GetActiveControlEffectTags_Implementation() override;
	virtual FGameplayTagContainer GetActiveStatusEffectTags_Implementation() override;
};
