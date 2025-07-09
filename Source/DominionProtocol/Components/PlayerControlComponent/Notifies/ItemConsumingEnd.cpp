// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemConsumingEnd.h"

#include "Components/PlayerControlComponent/PlayerControlComponent.h"

void UItemConsumingEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();

		if (IsValid(Owner))
		{
			UPlayerControlComponent* PlayerControlComponent = Owner->FindComponentByClass<UPlayerControlComponent>();

			if (IsValid(PlayerControlComponent))
			{
				PlayerControlComponent->DeactivateControlEffect(EffectTags::ConsumingItem);
			}
		}
	}
}
