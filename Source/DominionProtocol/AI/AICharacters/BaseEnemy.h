// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StatusComponent/StatusComponentUser.h"
#include "GameFramework/Character.h"
#include "Util/GameTagList.h"
#include "Player/Damagable.h"
#include "Player/EffectReceivable.h"
#include "BaseEnemy.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API ABaseEnemy :
public ACharacter, public IDamagable, public IEffectReceivable, public IStatusComponentUser
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI|State")
	bool bIsAttacking = true;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void LookAtTarget(const AActor* TargetActor);
	
	// Damagable
	virtual void OnAttacked_Implementation(const FAttackData& AttackData) override;

	// Attack Function
	UFUNCTION(BlueprintCallable, Category = "AI|Attack")
	void ExecutePattern(FGameplayTag SkillGroupTag);
	
	// EffectReceivable
	virtual FGameplayTagContainer GetActiveControlEffectTags_Implementation() override;
	virtual FGameplayTagContainer GetActiveStatusEffectTags_Implementation() override;

	// StatusComponentUser
	virtual void InitializeStatusComponent() override;
	virtual void OnDeath() override;
	virtual void OnGroggy() override;
};
