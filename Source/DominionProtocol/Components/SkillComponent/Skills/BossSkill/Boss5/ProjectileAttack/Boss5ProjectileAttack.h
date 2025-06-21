#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss5ProjectileAttack.generated.h"

class ACharacter;
class UProjectileMovementComponent;

UCLASS()
class DOMINIONPROTOCOL_API UBoss5ProjectileAttack : public UBaseSkill
{
	GENERATED_BODY()

public:
	UBoss5ProjectileAttack();

	virtual void Execute() override;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Projectile")
	TSubclassOf<AActor> ProjectileClass;

	//투사체 클래스 생성 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	FVector ProjectileSpawnOffset;

	//초기속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileInitialSpeed;

	//생명주기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float ProjectileLifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sound")
	TObjectPtr<USoundBase> FireSound;
};
