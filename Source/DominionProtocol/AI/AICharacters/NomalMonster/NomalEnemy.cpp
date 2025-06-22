// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacters/NomalMonster/NomalEnemy.h"

void ANomalEnemy::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

	SetLifeSpan(5.f);
}
