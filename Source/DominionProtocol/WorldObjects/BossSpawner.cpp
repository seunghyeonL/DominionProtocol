#include "WorldObjects/BossSpawner.h"
#include "DomiFramework/GameMode/BaseGameMode.h"
#include "AI/AICharacters/BossMonster/Boss4Enemy.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"

ABossSpawner::ABossSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABossSpawner::BeginPlay()
{
	Super::BeginPlay();
}


void ABossSpawner::SpawnBoss()
{
	if (!BossClass || !BossTag.IsValid())
	{
		Debug::Print(TEXT("BossSpawner: Return - !BossClass || !BossTag.IsValid()"));
		return;
	}
	AActor* SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossClass, GetActorLocation(),	GetActorRotation());
	if (BossClass->IsChildOf(ABoss4Enemy::StaticClass()))
	{
		if (ABoss4Enemy* Boss4 = Cast<ABoss4Enemy>(SpawnedBoss))
		{
			if (UDialogueManager* DialogueManager = Boss4->GetDialogueManager())
			{
				OnCreateDialogueManager.Broadcast(DialogueManager);
			}
		}
	}

	if (SpawnedBoss)
	{
		Debug::Print(TEXT("BossSpawner: Spawned boss"));
		if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->StartBattle(SpawnedBoss);
		}
	}

	if (BossTag == PawnTags::Boss2)
	{
		FadeElapsedTime = 0.0f;

		if (FadeMPC)
		{
			MPCInstance = GetWorld()->GetParameterCollectionInstance(FadeMPC);
		}

		GetWorld()->GetTimerManager().SetTimer(
			FadeTimerHandle, this, &ABossSpawner::UpdateFade, 0.05f, true
		);
	}
}

void ABossSpawner::UpdateFade()
{
	if (!FadeCurve || !MPCInstance) return;

	FadeElapsedTime += 0.05f;
	float Alpha = FadeElapsedTime / FadeDuration;
	float Value = FadeCurve->GetFloatValue(Alpha * FadeDuration);

	MPCInstance->SetScalarParameterValue("FadeRadius", Value);

	if (FadeElapsedTime >= FadeDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}
}