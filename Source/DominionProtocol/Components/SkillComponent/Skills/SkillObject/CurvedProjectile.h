#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DomiFramework/ObjectPooling/PoolableActorBase.h"
#include "CurvedProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FProjectileCurveSettings
{
	GENERATED_BODY()

public:
	// MidPoint가 시작점으로부터 떨어져 있어야 하는 비율 (0 ~ 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MinCurvePointDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MaxCurvePointDistance;

	// 곡선이 휘는 거리 (-: 왼쪽, +: 오른쪽)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MinCurveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MaxCurveRadius;

	// 선분 방향(X축) 기준으로 회전시킬 각도 (-: 아래, +: 위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MinAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MaxAngle;

	// 투사체 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed;

	// 투사체 생명주기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeSpan = 3.0f;
};

UCLASS()
class DOMINIONPROTOCOL_API ACurvedProjectile : public APoolableActorBase
{
	GENERATED_BODY()
	
public:	
	ACurvedProjectile();

	const FProjectileCurveSettings& GetCurveSettings() const { return CurveSettings; }
	void SetCurveSettings(const FProjectileCurveSettings& NewSettings) { CurveSettings = NewSettings; }

	AActor* GetInstigator() const { return Instigator; }
	void SetInstigator(AActor* NewInstigator) { Instigator = NewInstigator; }

	virtual void Tick(float DeltaTime) override;

	void OnObjectSpawn_Implementation() override;

	void OnObjectReturn_Implementation() override;

	void Launch(AActor* NewInstigator, const FVector& NewTargetPoint);

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

private:
	void MidPointCalculator();

	void CurveControl();

	FORCEINLINE FVector VInterpToConstant(const FVector& Current, const FVector& Target, float DeltaTime, float Speed);

	void DestroyProjectile();

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> Projectile;

	TObjectPtr<AActor> Instigator;

	FVector StartPoint;
	FVector TargetPoint;
	FVector MidPoint;
	FVector CurvePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileCurveSettings", meta = (AllowPrivateAccess = "true"))
	FProjectileCurveSettings CurveSettings;

	FTimerHandle DestroyTimerHandle;

};