#include "AI/AICharacters/BossMonster/Boss5Enemy.h"

ABoss5Enemy::ABoss5Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
	BossID = 5;
	PawnTag = PawnTags::Boss5;

	HeadArmComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("HeadArmComponent"));
	HeadArmComponent->SetupAttachment(RootComponent);

	RotateArmComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("RotateArmComponent"));
	RotateArmComponent->SetupAttachment(RootComponent);
}

