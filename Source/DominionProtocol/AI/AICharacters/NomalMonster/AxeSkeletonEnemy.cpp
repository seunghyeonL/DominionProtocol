// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/AxeSkeletonEnemy.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"

AAxeSkeletonEnemy::AAxeSkeletonEnemy()
{
	PawnTag = PawnTags::AxeSkeleton;

	EssenceAmount = 60.f;
}