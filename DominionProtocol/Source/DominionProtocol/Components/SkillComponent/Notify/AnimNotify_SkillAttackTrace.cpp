// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SkillComponent/Notify/AnimNotify_SkillAttackTrace.h"
#include "Util/DebugHelper.h"

void UAnimNotify_SkillAttackTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (IsValid(MeshComp))
	{
		AActor* Owner = MeshComp->GetOwner();

		if (IsValid(Owner))
		{
			// Owner를 캐스팅한다.
			// 캐스팅한 Owner의 스킬컴포넌트에 접근한다.
			// 스킬컴포넌트의 스킬에 접근한다.
			// 스킬의 AttackTrace 함수를 실행한다.

			Debug::Print(TEXT("Notify 실행"));
		}
	}
}
