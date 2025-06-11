// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/Notifies/AnimNotify_CylinderTrace.h"
#include "GameFramework/Character.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_CylinderTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (bShowDebug)
	{
		UWorld* World = GetWorld();
		if (!World) return;

		FVector Start = FVector::ZeroVector;
		if (MeshComp && MeshComp->GetOwner())
		{
			Start = MeshComp->GetOwner()->GetActorLocation() + TraceOffset;
		}

		DrawDebugCapsule(
			World,
			Start,
			HalfHeight,
			Radius,
			FQuat::Identity,
			FColor::Green,
			false, // true로 바꾸면 영구 표시
			2.0f   // 2초간 표시
		);
	}
	if (!IsValid(MeshComp)) return;

	ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (!IsValid(OwnerCharacter)) return;

	if (USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>())
	{
		if (UBaseSkill* Skill = SkillComponent->GetCurrentSkill())
		{
			Skill->AttackTrace_Cylinder(TraceOffset, Radius, HalfHeight);
		}
	}
}