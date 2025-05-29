#include "Components/SkillComponent/Skills/ZoneyaSkill.h"
#include "Util/GameTagList.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"

UZoneyaSkill::UZoneyaSkill()
{
	SkillTag = SkillTags::ZoneyaSkill;
	ControlEffectTag = EffectTags::UsingZoneya;
	Duration = 3.0f;
}

void UZoneyaSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UZoneyaSkill::Execute()
{
	check(IsValid(OwnerCharacter));

	StartZoneya();

	GetWorld()->GetTimerManager().SetTimer(ZoneyaTimerHandle, this, &UZoneyaSkill::EndZoneya, Duration, false);
}

void UZoneyaSkill::StartZoneya()
{
	OwnerCharacter->GetMesh()->bPauseAnims = true;

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
}

void UZoneyaSkill::EndZoneya()
{
	OwnerCharacter->GetMesh()->bPauseAnims = false;

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}
