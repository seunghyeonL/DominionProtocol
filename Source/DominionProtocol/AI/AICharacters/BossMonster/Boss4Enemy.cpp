// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/BossMonster/Boss4Enemy.h"
#include "Boss4Enemy.h"

ABoss4Enemy::ABoss4Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	BossID = 4;
	PawnTag = PawnTags::Boss4;
}
