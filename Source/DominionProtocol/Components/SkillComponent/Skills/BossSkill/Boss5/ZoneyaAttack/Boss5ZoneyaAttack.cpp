#include "Components/SkillComponent/Skills/BossSkill/Boss5/ZoneyaAttack/Boss5ZoneyaAttack.h"
#include "Util/DebugHelper.h"
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

	PC = Cast<APlayerController>(TargetCharacter->GetController());
	if (PC)
	{
		TargetCharacter->DisableInput(PC);
	}

	GetWorld()->GetTimerManager().SetTimer(ZoneyaTimerHandle, this, &UBoss5ZoneyaAttack::EndZoneya, 3.0f, false);

	UClass* TimePrisonClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/Blueprints/Boss/Boss5/TimePrison/BP_TimePrison.BP_TimePrison_C"));
	if (TimePrisonClass)
	{
		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(TimePrisonClass, FVector::ZeroVector, FRotator::ZeroRotator);

		if (SpawnedActor && TargetCharacter)
		{
			USceneComponent* TargetRoot = TargetCharacter->GetRootComponent();
			if (TargetRoot)
			{
				SpawnedActor->AttachToComponent(TargetRoot, FAttachmentTransformRules::KeepRelativeTransform);

				const float HalfHeight = Capsule->GetScaledCapsuleHalfHeight();
				SpawnedActor->SetActorRelativeLocation(FVector(0.f, 0.f, -HalfHeight)); // 위치 보정
			}
		}
	}
	else
	{
		Debug::PrintError(TEXT("UBoss5ZoneyaAttack::TimePrison Asset's location is not assigned"));
	}

	TargetCharacter->GetMesh()->bPauseAnims = true;

	if (!Sounds.IsEmpty() && Sounds.IsValidIndex(0))
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			Sounds[0],
			TargetCharacter->GetActorLocation()
		);
	}
}

void UBoss5ZoneyaAttack::EndZoneya()
{
	TargetCharacter->EnableInput(PC);
	TargetCharacter->GetMesh()->bPauseAnims = false;

	// 로봇 팔 공격 로직

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}