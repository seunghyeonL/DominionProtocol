// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/SkillObject/CurvedProjectile.h"
#include "Boss3Rock.generated.h"

class UGeometryCollectionComponent;
class UNiagaraSystem;
class USoundBase;

UCLASS()
class DOMINIONPROTOCOL_API ABoss3Rock : public ACurvedProjectile
{
	GENERATED_BODY()
public:
	ABoss3Rock();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Visual")
	FRotator RotationPerSecond = FRotator(0, 0, 300.f);
protected:

	virtual void DestroyProjectile() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rock")
	UGeometryCollectionComponent* RockMesh;

	/** 파편 튈 때 이펙트(선택사항) */
	UPROPERTY(EditDefaultsOnly, Category = "Break")
	UNiagaraSystem* BreakFX = nullptr;

	/** 파편이 남아 있을 시간 */
	UPROPERTY(EditDefaultsOnly, Category = "Break", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float BreakRemainTime = 2.f;
};
