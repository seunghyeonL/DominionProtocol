#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "PhysicalSurfaceTypeData.generated.h"

USTRUCT(BlueprintType)
struct FPhysicalSurfaceTypeData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> FootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> FootStepVfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UNiagaraSystem> HitVfx;
};
