#include "Components/SkillComponent/Skills/MagicSkill/MagicZoneyaSkill.h"
#include "Util/GameTagList.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "DomiFramework/GameState/BaseGameState.h"
#include "Components/SkillComponent/SkillComponent.h"
#include "Components/PlayerControlComponent/PlayerControlComponent.h"

UMagicZoneyaSkill::UMagicZoneyaSkill()
{
	SkillTag = SkillTags::MagicZoneyaSkill;
	ControlEffectTag = EffectTags::UsingZoneya;
	Duration = 3.0f;
}

void UMagicZoneyaSkill::Initialize(ACharacter* Instigator)
{
	Super::Initialize(Instigator);
}

void UMagicZoneyaSkill::Execute()
{
	check(IsValid(OwnerCharacter));

	StartZoneya();

	GetWorld()->GetTimerManager().SetTimer(ZoneyaTimerHandle, this, &UMagicZoneyaSkill::EndZoneya, Duration, false);
}

void UMagicZoneyaSkill::StartZoneya()
{
	FVector SpawnLocation = OwnerCharacter->GetActorLocation();

	UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent();
	float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();

	if (NiagaraParticle[0])
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraParticle[0],
			SpawnLocation - FVector(0.f, 0.f, HalfHeight),
			FRotator::ZeroRotator,     // 회전 고정
			FVector(1.0f),             // 스케일 (크기)
			true,                      // 자동 파괴 여부
			true,                      // AutoActivate
			ENCPoolMethod::None,       // 풀링 설정
			true                       // PreCullCheck
		);
	}

	OwnerCharacter->GetMesh()->bPauseAnims = true;

	if (Sound[0])
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sound[0],
			SpawnLocation
		);
	}
}

void UMagicZoneyaSkill::EndZoneya()
{
	OwnerCharacter->GetMesh()->bPauseAnims = false;

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}
