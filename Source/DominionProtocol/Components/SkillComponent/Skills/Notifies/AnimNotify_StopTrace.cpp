// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/Notifies/AnimNotify_StopTrace.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "Util/GameTagList.h"

void UAnimNotify_StopTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();

		if (IsValid(Owner))
		{
			UMnhTracerComponent* MnhTracerComponent = Owner->FindComponentByClass<UMnhTracerComponent>();

			if (IsValid(MnhTracerComponent))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(ItemTags::BasicWeapon);

				MnhTracerComponent->StopTracers(TagContainer);
			}
		}
	}
}
