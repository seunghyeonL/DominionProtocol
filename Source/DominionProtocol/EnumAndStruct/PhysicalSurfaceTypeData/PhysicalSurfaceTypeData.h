#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "PhysicalSurfaceTypeData.generated.h"

USTRUCT(BlueprintType)
struct FPhysicalSurfaceTypeData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> FootStepSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> FootStepVfx;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UNiagaraSystem> HitVfx;
};
