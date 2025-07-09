#include "Components/SkillComponent/Skills/MagicSkill/MagicInfiniteStaminaSkill.h"
#include "NiagaraFunctionLibrary.h"
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
	FRotator SpawnRotation = OwnerCharacter->GetActorRotation();

	if (!NiagaraParticles.IsEmpty() && NiagaraParticles.IsValidIndex(0))
	{
		UNiagaraSystem* NiagaraSystem = NiagaraParticles[0];
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			NiagaraSystem,
			OwnerCharacter->GetRootComponent(),
			NAME_None,
			FVector(0.f, 0.f, -70.f),
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true,
			true
		);
	}

	if (!Sounds.IsEmpty() && Sounds.IsValidIndex(0))
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sounds[0],
			SpawnLocation
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