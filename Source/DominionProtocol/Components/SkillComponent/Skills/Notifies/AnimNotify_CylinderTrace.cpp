// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/Notifies/AnimNotify_CylinderTrace.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_CylinderTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!IsValid(MeshComp)) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!IsValid(Owner)) return;

	if (USkillComponent* SkillComponent = Owner->FindComponentByClass<USkillComponent>())
	{
		if (UBaseSkill* Skill = SkillComponent->GetCurrentSkill())
		{
			Skill->AttackTrace_Cylinder(TraceOffset, Radius, HalfHeight);
		}
	}
}