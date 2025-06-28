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

	if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GM->OnEndBattle.AddUObject(this, &ABossSpawner::ResetFade);
	}
}


void ABossSpawner::SpawnBoss()
{
	if (!BossClass || !BossTag.IsValid())
	{
		Debug::Print(TEXT("BossSpawner: Return - !BossClass || !BossTag.IsValid()"));
		return;
	}

	if (BossTag == PawnTags::Boss2)
	{
		GetWorld()->GetTimerManager().SetTimer(
			PreFadeDelayHandle,
			this,
			&ABossSpawner::StartFade,
			Boss2SpawnDelay,
			false
		);
	}
	else if (BossTag == PawnTags::Boss4)
	{
		StartFade();
		UE_LOG(LogTemp, Display, TEXT("Boss4"));
	}
	else
	{
		SpawnBossInternal();
	}
}

void ABossSpawner::StartFade()
{

	FadeElapsedTime = 0.0f;

	if (FadeMPC)
	{
		MPCInstance = GetWorld()->GetParameterCollectionInstance(FadeMPC);
	}

	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle, 
		this, 
		&ABossSpawner::UpdateFade, 
		0.05f, 
		true
	);
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

		SpawnBossInternal();
	}
}

void ABossSpawner::SpawnBossInternal()
{
	SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossClass, GetActorLocation(), GetActorRotation());

	if (BossTag == PawnTags::Boss4)
	{
		if (ABoss4Enemy* Boss4 = Cast<ABoss4Enemy>(SpawnedBoss))
		{
			if (UDialogueManager* DialogueManager = Boss4->GetDialogueManager())
			{
				OnCreateDialogueManager.Broadcast(DialogueManager);
			}
		}
	}

	if (SpawnedBoss.IsValid())
	{
		Debug::Print(TEXT("BossSpawner: Spawned boss"));
		if (ABaseGameMode* GM = Cast<ABaseGameMode>(UGameplayStatics::GetGameMode(this)))
		{
			GM->StartBattle(SpawnedBoss.Get());
		}
	}
}

void ABossSpawner::ResetFade(AActor* Boss)
{
	FadeElapsedTime = 0.0f;

	if (FadeMPC)
	{
		MPCInstance = GetWorld()->GetParameterCollectionInstance(FadeMPC);

		if (FadeMPC2)
		{
			MPCInstance2 = GetWorld()->GetParameterCollectionInstance(FadeMPC2);

			float StartValue = FadeCurve->GetFloatValue(0.0f);
			MPCInstance->SetScalarParameterValue("FadeRadius", StartValue);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
		FadeTimerHandle,
		this,
		&ABossSpawner::UpdateReverseFade,
		0.05f,
		true
	);
}

void ABossSpawner::UpdateReverseFade()
{
	if (!FadeCurve || !MPCInstance) return;

	FadeElapsedTime += 0.05f;
	float Alpha = FadeElapsedTime / FadeDuration;

	float Value = FadeCurve->GetFloatValue((1.0f - Alpha) * FadeDuration);

	if (FadeMPC2)
	{
		MPCInstance2->SetScalarParameterValue("FadeRadius", Value);
	}
	else
	{
		MPCInstance->SetScalarParameterValue("FadeRadius", Value);
	}

	if (FadeElapsedTime >= FadeDuration)
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeTimerHandle);
	}
}