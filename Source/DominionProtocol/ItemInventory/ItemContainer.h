// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/Damagable.h"
#include "ItemContainer.generated.h"

class UStaticMeshComponent;
class ABaseItem;

UCLASS()
class DOMINIONPROTOCOL_API AItemContainer : public AActor, public IDamagable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemContainer();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ContainerShellMesh; // 파괴 전 오크통/상자의 온전한 스태틱 메시

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destructible")
	float MaxHealth = 50.0f; // 컨테이너의 최대 체력

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destructible")
	float CurrentHealth; // 컨테이너의 현재 체력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cleanup")
	float DestroyDelayAfterBreak = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	TArray<TSubclassOf<ABaseItem>> ItemsToSpawn;

	UFUNCTION()
	void OnHealthZeroed(); // 체력이 0이 되었을 때 호출될 함수

	UFUNCTION()
	void DestroyContainerActor();

	UFUNCTION()
	void SpawnRandomItems();

	UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
	void PlayShakeAnimation();
public:
	virtual void Tick(float DeltaTime) override;

	// IDamagable 인터페이스 구현
	virtual void OnAttacked_Implementation(const FAttackData& AttackData) override;
};
