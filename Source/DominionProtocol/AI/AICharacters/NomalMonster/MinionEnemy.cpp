// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/MinionEnemy.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhTracerComponent.h"
#include "../Plugins/MissNoHit/Source/MissNoHit/Public/MnhComponents.h"

AMinionEnemy::AMinionEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	PawnTag = PawnTags::Minion;
}