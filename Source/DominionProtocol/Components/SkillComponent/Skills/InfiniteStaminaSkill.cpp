#include "Components/SkillComponent/Skills/InfiniteStaminaSkill.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/StatusComponent/StatusComponent.h"

UInfiniteStaminaSkill::UInfiniteStaminaSkill()
{
	SkillTag = SkillTags::InfiniteStaminaSkill;
	Duration = 10.0f;
}

void UInfiniteStaminaSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UInfiniteStaminaSkill::Execute()
{
	check(IsValid(OwnerCharacter));

	Start();

	GetWorld()->GetTimerManager().SetTimer(InfiniteTimerHandle, this, &UInfiniteStaminaSkill::End, Duration, false);
}

void UInfiniteStaminaSkill::Start()
{
	StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();
	check(StatusComponent);

	StatusComponent->SwitchInfiniteStaminaMode();

	UWorld* World = GetWorld();
	check(World);

	FVector SpawnLocation = OwnerCharacter->GetActorLocation();

	ABaseGameState* BaseGameState = World->GetGameState<ABaseGameState>();
	check(BaseGameState);

	if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
	{
		Sound[0] = SkillData->Sound[0];
		if (Sound[0])
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				Sound[0],
				SpawnLocation
			);
		}
	}

	if (FSkillData* SkillData = BaseGameState->GetSkillData(SkillTag))
	{
		Particle[0] = SkillData->Particle[0];
		if (Particle[0])
		{
			UParticleSystemComponent* PSC = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				Particle[0],
				SpawnLocation,
				FRotator::ZeroRotator,
				true
			);
		}
	}

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}

void UInfiniteStaminaSkill::End()
{
	StatusComponent->SwitchInfiniteStaminaMode();
	Debug::PrintError(TEXT("UInfiniteStaminaSkill: Infinite stamina off."));
}