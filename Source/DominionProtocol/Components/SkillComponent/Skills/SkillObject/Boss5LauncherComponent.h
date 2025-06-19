#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Boss5LauncherComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOMINIONPROTOCOL_API UBoss5LauncherComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBoss5LauncherComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Launcher")
	TSubclassOf<AActor> ProjectileClass; // 발사할 투사체

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Launcher")
	float ProjectileSpeed; // 투사체의 초기 속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Launcher")
	FName LaunchSocketName; //발사할 메시의 소켓 이름

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Launcher")
	float AttackCooldown; // 발사 후 쿨타임

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Launcher")
	bool bCanLaunchProjectile; // 투사체 발사 가능 여부

	// 목표를 설정하고 추적하여 자동으로 발사하도록 지시
	UFUNCTION(BlueprintCallable, Category = "Projectile Launcher")
	void SetAndTrackTarget(AActor* NewTargetActor);

	// 현재 추적 중인 목표를 해제하여 더 이상 자동으로 발사하지 않도록
	UFUNCTION(BlueprintCallable, Category = "Projectile Launcher")
	void ClearTarget();

	//목표로 발사
	UFUNCTION(BlueprintCallable, Category = "Projectile Launcher")
	void LaunchProjectileAtTarget(AActor* TargetActor);

	//해당 방향으로 발사
	UFUNCTION(BlueprintCallable, Category = "Projectile Launcher")
	void LaunchProjectileInDirection(const FVector& LaunchDirection);

protected:
	// 쿨타임 관리
	FTimerHandle CooldownTimerHandle;

	UPROPERTY(BlueprintReadWrite,EditDefaultsOnly)
	AActor* CurrentTargetActor;

	UFUNCTION()
	void OnCooldownFinished();

	// 투사체 스폰
	UFUNCTION(BlueprintCallable, Category = "Projectile Launcher")
	void SpawnProjectile(const FVector& SpawnLocation, const FRotator& SpawnRotation);

		
};
