// Fill out your copyright notice in the Description page of Project Settings.


#include "ProtoNormalEnemy.h"
#include "Components/StatusComponent/StatusComponent.h"
#include "Components/SkillComponent/SkillComponent.h"


// Sets default values
AProtoNormalEnemy::AProtoNormalEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnTag = PawnTags::ProtoNormal;
}

// Called when the game starts or when spawned
void AProtoNormalEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProtoNormalEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AProtoNormalEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AProtoNormalEnemy::OnGroggy()
{
	check(SkillComponent);
	Super::OnGroggy();
	
	SkillComponent->StopSkill();
	StatusComponent->ActivateStatusEffect(EffectTags::Groggy, 0.f, 3.f);
}

void AProtoNormalEnemy::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();
	
	if (auto* MeshComp = GetMesh())
	{
		MeshComp->SetSimulatePhysics(true);  // 물리 시뮬레이션 시작
		MeshComp->SetCollisionProfileName("Ragdoll");  // 적절한 콜리전 프로필 설정
		MeshComp->bPauseAnims = true;        // 애니메이션 중지
		MeshComp->bNoSkeletonUpdate = false;
	}
}


