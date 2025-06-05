// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SkillComponent/Skills/NomalMonsterSkill/Wood/WoodHurricaneKick.h"
#include "Util/GameTagList.h"
#include "AI/AICharacters/NomalMonster/WoodEnemy.h"

UWoodHurricaneKick::UWoodHurricaneKick()
{
	SkillTag = SkillTags::WoodHurricaneKick;
}

void UWoodHurricaneKick::Execute()
{
	if (AWoodEnemy* WoodEnemy = Cast<AWoodEnemy>(OwnerCharacter))
	{
		FRotator CurrentRotation = WoodEnemy->GetActorRotation();
		FRotator OffsetRotation = FRotator(0.f, -30.f, 0.f);
		WoodEnemy->SetActorRotation(CurrentRotation + OffsetRotation);
	}
	Super::Execute();
}