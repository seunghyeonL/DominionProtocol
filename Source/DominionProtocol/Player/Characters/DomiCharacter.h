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
	
public:
	ADomiCharacter();

	//Getter
	FORCEINLINE AActor* GetCurrentInteractableObject() const;
	 
	//Setter
	FORCEINLINE void SetCurrentInteractableObject(AActor* NewActor) { InteractableObject = NewActor; }
	
protected:
	// Bind Matched Input Functions
	void BindInputFunctions();
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnDeath();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	// ControlComponentUser
	virtual FGameplayTagContainer GetActiveControlEffectTags() override;
	FORCEINLINE virtual UPlayerControlComponent* GetPlayerControlComponent() const override { return ControlComponent; }
	virtual void SkillStart(FGameplayTag ControlEffectTag) override;
	virtual void SkillEnd(FGameplayTag ControlEffectTag) override;

	// StatusComponentUser
	FORCEINLINE virtual UStatusComponent* GetStatusComponent() const override { return StatusComponent; }
	virtual void InitializeStatusComponent() override;
	virtual FGameplayTagContainer GetActiveStatusEffectTags() override;

	// SkillComponentUser
	FORCEINLINE virtual USkillComponent* GetSkillComponent() const override { return SkillComponent; }
	virtual void InitializeSkillComponent() override;
	virtual void ExecuteSkill(FGameplayTag SkillGroupTag) override;
	
	// Damagable
	virtual void OnAttacked_Implementation(const FAttackData& AttackData) override;

	// EffectReceivable
	virtual void ShowControlEffectTags_Implementation() override;
	virtual void ShowStatusEffectTags_Implementation() override;

private:
	UPROPERTY()
	AActor* InteractableObject;

	// === Dash 실행 함수 ===
public:

	/** Dash 가능 여부 (스태미나 충분한지) */
	bool HasEnoughStamina() const;

	/** Dash 시작: 스태미나 차감, 무적 시작, 이펙트 적용 */
	bool StartDash();

	/** Dash 종료: 무적 해제 */
	void EndDash();

	/** 무적 상태 on/off */
	void SetInvincible(bool bInvincible);

	/** Dash 방향 계산 (입력 없으면 뒤로) */
	FVector GetDashDirection() const;

	// === Dash 수치 설정값 ===
	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDistance = 600.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashDuration = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashStaminaCost = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Dash")
	float DashMovedDistance = 0.f;

	UPROPERTY()
	FVector DashMoveDirection;


	// === Dash 이동용 변수 ===

	UPROPERTY()
	float DashElapsed = 0.f;

	// === 기타 상태 ===

	UPROPERTY(VisibleAnywhere, Category = "State")
	bool bIsInvincible = false;

};

