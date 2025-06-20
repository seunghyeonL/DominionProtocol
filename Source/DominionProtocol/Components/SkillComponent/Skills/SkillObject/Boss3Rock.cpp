// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/SkillObject/Boss3Rock.h"


void ABoss3Rock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(Projectile))
	{
		Projectile->AddLocalRotation(RotationPerSecond * DeltaTime);
	}
}