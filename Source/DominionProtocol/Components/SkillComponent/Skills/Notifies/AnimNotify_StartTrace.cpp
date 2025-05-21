// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/Notifies/AnimNotify_StartTrace.h"
#include "Components/SkillComponent/SkillComponent.h"

void UAnimNotify_StartTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();

		if (IsValid(Owner))
		{
			USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>();

			if (IsValid(SkillComponent))
			{
				UBaseSkill* BaseSkill = SkillComponent->GetCurrentSkill();

				if (IsValid(BaseSkill))
				{
					BaseSkill->StartTrace();
				}
			}
		}
	}
}
