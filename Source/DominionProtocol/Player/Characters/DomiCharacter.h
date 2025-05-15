// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Damagable.h"
#include "../EffectReceivable.h"
#include "Components/PlayerControlComponent/ControlComponentUser.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "Util/DebugHelper.h"
#include "Components/SkillComponent/SkillComponentUser.h"
#include "DomiCharacter.generated.h"

class UStatusComponent;
class USkillComponent;
class UItemComponent;
class UPlayerControlComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
struct FAttackData;

class UMnhTracerComponent;
class UMnhBoxComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAddInteractableActor, TArray<AActor*>)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRemoveInteractableActor, TArray<AActor*>)

UCLASS()
class DOMINIONPROTOCOL_API ADomiCharacter :
public ACharacter, public IDamagable, public IEffectReceivable, public IControlComponentUser, public IStatusComponentUser, public ISkillComponentUser
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerControlComponent> ControlComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatusComponent> StatusComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillComponent> SkillComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMnhTracerComponent> AttackTraceComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UItemComponent* ItemComponent;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag", meta = (AllowPrivateAccess = "true"))
	FGameplayTag PawnTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer InvincibilityTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer ParriedTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UStaticMeshComponent> TempWeapon;			// 추후에 무기 생기면 수정 필요

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<UMnhBoxComponent> WeaponTraceBox;	// 추후에 무기 쪽으로 이동 필요
	
	
public:
	ADomiCharacter();

	FOnAddInteractableActor OnAddInteractableActor;
	FOnRemoveInteractableActor OnRemoveInteractableActor;

	//Getter
	FORCEINLINE AActor* GetCurrentInteractableActor() const;
	 
	//Setter
	FORCEINLINE void SetCurrentInteractableObject(AActor* NewActor) { InteractableActor = NewActor; }

	void AddInteractableActor(AActor* AddInteractableActor);
	void RemoveInteractableActor(AActor* RemoveInteractableActor);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// ControlComponentUser
	virtual FGameplayTagContainer& GetActiveControlEffectTags() override;
	FORCEINLINE virtual UPlayerControlComponent* GetPlayerControlComponent() const override { return ControlComponent; }
	FORCEINLINE virtual void SetLastMovementVector(const FVector& InLastMovementVector) override { LastMovementVector = InLastMovementVector; }
	FORCEINLINE virtual FVector GetLastMovementVector() const override { return LastMovementVector; }
	FORCEINLINE virtual void ResetLastMovementVector() override { LastMovementVector = FVector::ZeroVector; }
	virtual void SkillStart(FGameplayTag ControlEffectTag) override;
	virtual void SkillEnd(FGameplayTag ControlEffectTag) override;

	// StatusComponentUser
	FORCEINLINE virtual UStatusComponent* GetStatusComponent() const override { return StatusComponent; }
	virtual void InitializeStatusComponent() override;
	virtual FGameplayTagContainer& GetActiveStatusEffectTags() override;

	// SkillComponentUser
	FORCEINLINE virtual USkillComponent* GetSkillComponent() const override { return SkillComponent; }
	virtual void InitializeSkillComponent() override;
	virtual void ExecuteSkill(FGameplayTag SkillGroupTag) override;
	
	// Damagable
	virtual void OnAttacked_Implementation(const FAttackData& AttackData) override;

	// EffectReceivable
	virtual void ShowControlEffectTags_Implementation() override;
	virtual void ShowStatusEffectTags_Implementation() override;

protected:
	// Bind Matched Input Functions
	void BindInputFunctions();
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnDeath();
	void Parrying(const FAttackData& IncomingAttackData);


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerControlComponent> ControlComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStatusComponent> StatusComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkillComponent> SkillComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UItemComponent* ItemComponent;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer InvincibilityTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "States", meta = (AllowPrivateAccess = "true"))
	FGameplayTagContainer ParriedTags;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tag", meta = (AllowPrivateAccess = "true"))
	FGameplayTag PawnTag;

private:
	UPROPERTY()
	AActor* InteractableActor;

	UPROPERTY()
	TArray<AActor*> InteractableActorArray = {};

	// 마지막 이동 입력 방향벡터
	FVector LastMovementVector;
};

