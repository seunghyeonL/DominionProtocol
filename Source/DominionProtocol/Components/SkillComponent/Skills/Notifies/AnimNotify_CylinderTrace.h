// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_CylinderTrace.generated.h"

/**
 * 
 */
UCLASS()
class DOMINIONPROTOCOL_API UAnimNotify_CylinderTrace : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	// 트레이스 위치 (캐릭터 기준 상대 위치)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	FVector TraceOffset = FVector(100.f, 0.f, 0.f);

	// 원기둥의 반지름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float Radius = 150.f;

	// 원기둥의 절반 높이 (Z축 방향 두께)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float HalfHeight = 50.f;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bShowDebug = true;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
