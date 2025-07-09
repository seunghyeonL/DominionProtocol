// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/Notifies/AnimNotify_MoveCharacter.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/SkillComponent/Skills/BossSkill/Boss1/Boss1JumpAttack.h"

void UAnimNotify_MoveCharacter::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
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
					UBoss1JumpAttack* Boss1JumpAttack = Cast<UBoss1JumpAttack>(BaseSkill);

					if (IsValid(Boss1JumpAttack))
					{
						Boss1JumpAttack->MoveToLocation();
					}
				}
			}
		}
	}
}
