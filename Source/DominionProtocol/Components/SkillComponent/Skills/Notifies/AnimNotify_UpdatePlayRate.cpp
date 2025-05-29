// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_UpdatePlayRate.h"

void UAnimNotify_UpdatePlayRate::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (auto* AnimInstance = MeshComp->GetAnimInstance())
	{
		if (auto* CurrentMontage = AnimInstance->GetCurrentActiveMontage())
		{
			AnimInstance->Montage_SetPlayRate(CurrentMontage, this->PlayRate);
		}
	}
}
