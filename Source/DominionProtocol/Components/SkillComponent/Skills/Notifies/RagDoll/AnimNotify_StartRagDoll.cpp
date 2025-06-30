// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_StartRagDoll.h"

void UAnimNotify_StartRagDoll::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (IsValid(MeshComp))
	{
		MeshComp->SetSimulatePhysics(true);  // 물리 시뮬레이션 시작
		MeshComp->SetCollisionProfileName("Ragdoll");  // 적절한 콜리전 프로필 설정
		MeshComp->bPauseAnims = true;        // 애니메이션 중지
		MeshComp->bNoSkeletonUpdate = false;
	}
}
