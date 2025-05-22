#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DomiFramework/ObjectPooling/PoolableActorBase.h"
#include "CurvedProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USkillComponent;
class UObjectPoolSubsystem;

USTRUCT(BlueprintType)
struct FProjectileCurveSettings
{
	GENERATED_BODY()

public:
	// MidPoint가 시작점으로부터 떨어져 있어야 하는 비율 (0 ~ 1)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//float MinCurvePointDistance;
	float MinCurvePointDistance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//float MaxCurvePointDistance;
	float MaxCurvePointDistance = 0.0f;

	// 곡선이 휘는 거리 (-: 왼쪽, +: 오른쪽)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//float MinCurveRadius;
	float MinCurveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//float MaxCurveRadius;
	float MaxCurveRadius;

	// 선분 방향(X축) 기준으로 회전시킬 각도 (-: 아래, +: 위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//float MinAngle;
	float MinAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//float MaxAngle;
	float MaxAngle;

	// 투사체 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	//float ProjectileSpeed;
	float ProjectileSpeed;

	// 투사체 생명주기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeSpan;
};

UCLASS()
class DOMINIONPROTOCOL_API ACurvedProjectile : public APoolableActorBase
{
	GENERATED_BODY()
	
public:	
	ACurvedProjectile();;

	const FProjectileCurveSettings& GetCurveSettings() const { return CurveSettings; }
	void SetCurveSettings(const FProjectileCurveSettings& NewSettings) { CurveSettings = NewSettings; }

	virtual void SetOwnerCharacter(AActor* NewOwnerCharacter);

	virtual void Tick(float DeltaTime) override;

	void OnObjectSpawn_Implementation() override;

	void OnObjectReturn_Implementation() override;

	void SetLaunchPath(AActor* NewInstigator, AActor* NewTargetActor);

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill")
	TObjectPtr<AActor> OwnerCharacter;

private:
	void MidPointCalculator();

	void CurveControl();

	void DestroyProjectile();

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> Projectile;

	UPROPERTY()
	TObjectPtr<USkillComponent> SkillComponent;

	AActor* Instigator;
	AActor* TargetActor;

	// 투사체 생성 지점
	FVector StartPoint;

	// 타겟 지점
	FVector TargetPoint;

	FVector MidPoint;
	FVector CurvePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileCurveSettings", meta = (AllowPrivateAccess = "true"))
	FProjectileCurveSettings CurveSettings;

	FTimerHandle DestroyTimerHandle;
	
	UPROPERTY()
	UParticleSystem* ImpactParticle;

	bool bIsTargetMove = false;
	bool bIsInitialize = false;
	UPROPERTY()
	UObjectPoolSubsystem* ObjectPoolSubsystem;
};