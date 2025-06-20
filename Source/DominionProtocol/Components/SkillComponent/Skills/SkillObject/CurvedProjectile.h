#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "Util/BattleDataTypes.h"
#include "CurvedProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class USkillComponent;
class UMagicCurvedProjectileSkill;

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

	// 투사체가 커브를 계속할 시간
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float CurveDuration;

	// 투사체 생명주기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeSpan;
};

UCLASS(Blueprintable)
class DOMINIONPROTOCOL_API ACurvedProjectile : public AActor //APoolableActorBase
{
	GENERATED_BODY()

public:
	ACurvedProjectile();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetLaunchPath(AActor* NewInstigator, AActor* NewTargetActor);

	UPROPERTY()
	UMagicCurvedProjectileSkill* SkillOwner = nullptr;

	UPROPERTY()
	FGameplayTag SkillTag;

	UPROPERTY()
	FAttackData AttackData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bUseLookAtRotation = true;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* DestroySound = nullptr;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	TObjectPtr<UStaticMeshComponent> Projectile;
protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	virtual void ApplyAttackToHitActor(const FHitResult& HitResult, const float DeltaTime);

	virtual bool CheckParry(const FHitResult& HitResult);

	virtual void OnParried(AActor* ParryActor);

private:
	void MidPointCalculator();

	void CurveControl();

	void DestroyProjectile();

	void UpdateCurveMovement(float DeltaTime);
	void FixTargetPoint();
	bool HasReachedTarget();
	void OnReachTarget();
	void MoveInStraightLine(float DeltaTime);

	UPROPERTY(VisibleDefaultsOnly, Category = "Collision")
	TObjectPtr<USphereComponent> SphereCollision;

	//UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	//TObjectPtr<UStaticMeshComponent> Projectile;

	UPROPERTY()
	TObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileCurveSettings", meta = (AllowPrivateAccess = "true"))
	FProjectileCurveSettings CurveSettings;

	UPROPERTY()
	TObjectPtr<APawn> InstigatorPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> TargetActor;

	// 투사체 생성 지점
	FVector StartPoint;

	// 타겟 지점
	FVector TargetPoint;

	FVector MidPoint;
	FVector CurvePoint;

	FTimerHandle DestroyTimerHandle;

	bool bIsTargetMove = false;
	bool bIsInitialize = false;
	bool bCurveFixed = false;  // 커브가 고정되었는지 확인하는 플래그
	bool bReachedTarget = false;  // 타겟에 도달했는지 확인하는 플래그
	FVector DirectionVector;  // 타겟 도달 후 직진할 방향 벡터

	float ElapsedTime = 0.0f;  // 경과 시간 추적
};