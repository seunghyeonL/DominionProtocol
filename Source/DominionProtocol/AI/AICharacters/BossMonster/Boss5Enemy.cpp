#include "AI/AICharacters/BossMonster/Boss5Enemy.h"

ABoss5Enemy::ABoss5Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	BossID = 5;
	PawnTag = PawnTags::Boss5;
}