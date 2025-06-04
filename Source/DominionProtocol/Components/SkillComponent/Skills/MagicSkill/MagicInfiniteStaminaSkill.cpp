#include "Components/SkillComponent/Skills/MagicSkill/MagicInfiniteStaminaSkill.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/StatusComponent/StatusComponent.h"

UMagicInfiniteStaminaSkill::UMagicInfiniteStaminaSkill()
{
	SkillTag = SkillTags::MagicInfiniteStaminaSkill;
	Duration = 10.0f;
}

void UMagicInfiniteStaminaSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UMagicInfiniteStaminaSkill::Execute()
{
	check(IsValid(OwnerCharacter));

	Start();

	GetWorld()->GetTimerManager().SetTimer(InfiniteTimerHandle, this, &UMagicInfiniteStaminaSkill::End, Duration, false);
}

void UMagicInfiniteStaminaSkill::Start()
{
	StatusComponent = OwnerCharacter->FindComponentByClass<UStatusComponent>();
	check(StatusComponent);

	StatusComponent->SwitchInfiniteStaminaMode();

	FVector SpawnLocation = OwnerCharacter->GetActorLocation();

	if (Sound[0])
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sound[0],
			SpawnLocation
		);
	}

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

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}

void UMagicInfiniteStaminaSkill::End()
{
	StatusComponent->SwitchInfiniteStaminaMode();
	Debug::PrintError(TEXT("UInfiniteStaminaSkill: Infinite stamina off."));
}