#include "Components/SkillComponent/Skills/BossSkill/Boss5/ZoneyaAttack/Boss5ZoneyaAttack.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "AI/AIControllers/Boss5AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SkillComponent/SkillComponent.h"

UBoss5ZoneyaAttack::UBoss5ZoneyaAttack()
{
	SkillTag = SkillTags::Boss5ZoneyaAttack;
}

void UBoss5ZoneyaAttack::Execute()
{
	if (!OwnerCharacter) return;

	AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
	if (!AIController) return;

	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (!BBComp) return;

	TargetCharacter = Cast<ACharacter>(BBComp->GetValueAsObject(FName("TargetActor")));
	if (!TargetCharacter) return;

	UCapsuleComponent* Capsule = TargetCharacter->GetCapsuleComponent();
	if (!Capsule) return;

	const FVector SpawnLocation = TargetCharacter->GetActorLocation() - FVector(0.f, 0.f, Capsule->GetScaledCapsuleHalfHeight());

	PC = Cast<APlayerController>(TargetCharacter->GetController());
	if (PC)
	{
		PC->SetIgnoreMoveInput(true);
	}

	GetWorld()->GetTimerManager().SetTimer(ZoneyaTimerHandle, this, &UBoss5ZoneyaAttack::EndZoneya, 3.0f, false);

	if (!NiagaraParticles.IsEmpty() && NiagaraParticles.IsValidIndex(0))
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			NiagaraParticles[0],
			SpawnLocation,
			FRotator::ZeroRotator,
			FVector(1.0f),
			true,
			true,
			ENCPoolMethod::None,
			true
		);
	}

	TargetCharacter->GetMesh()->bPauseAnims = true;

	if (!Sounds.IsEmpty() && Sounds.IsValidIndex(0))
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sounds[0],
			SpawnLocation
		);
	}
}

void UBoss5ZoneyaAttack::EndZoneya()
{
	PC->SetIgnoreMoveInput(false);
	TargetCharacter->GetMesh()->bPauseAnims = false;

	// 로봇 팔 공격 로직

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}