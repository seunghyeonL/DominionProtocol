// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlayerParryEnd.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"

void UAnimNotify_PlayerParryEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();

		if (IsValid(Owner))
		{
			if (auto ControlComponent = Owner->FindComponentByClass<UPlayerControlComponent>())
			{
				ControlComponent->DeactivateControlEffect(EffectTags::Parry);
			}
		}
	}
}
