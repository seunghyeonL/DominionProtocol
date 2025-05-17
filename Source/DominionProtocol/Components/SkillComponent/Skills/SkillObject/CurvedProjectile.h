#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CurvedProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;

USTRUCT(BlueprintType)
struct FProjectileCurveSettings
{
	GENERATED_BODY()

public:
	// MidPoint가 시작점으로부터 떨어져 있어야 하는 비율 (0 ~ 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	float MinCurvePointDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	float MaxCurvePointDistance;

	// 곡선이 휘는 거리 (-: 왼쪽, +: 오른쪽)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	float MinCurveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	float MaxCurveRadius;

	// 선분 방향(X축) 기준으로 회전시킬 각도 (-: 아래, +: 위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	float MinAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	float MaxAngle;

	// 투사체 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	float ProjectileSpeed;
};

UCLASS()
class DOMINIONPROTOCOL_API ACurvedProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACurvedProjectile();

	const FProjectileCurveSettings& GetCurveSettings() const { return CurveSettings; }
	void SetCurveSettings(const FProjectileCurveSettings& NewSettings) { CurveSettings = NewSettings; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void MidPointCalculator();

	void CurveControl();

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> Projectile;

	FVector TargetLocation;
	FVector MidPoint;
	FVector CurvePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileCurveSettings", meta = (AllowPrivateAccess = "true"))
	FProjectileCurveSettings CurveSettings;
};