#include "Components/SkillComponent/Skills/BossSkill/Boss5/SpawnEnemyAttack/Boss5SpawnEnemyAttack.h"
#include "Util/DebugHelper.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/AIControllers/Boss5AIController.h"
#include "Components/SkillComponent/SkillComponent.h"

UBoss5SpawnEnemyAttack::UBoss5SpawnEnemyAttack()
{
	SkillTag = SkillTags::Boss5SpawnEnemyAttack;
}

void UBoss5SpawnEnemyAttack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!SpawnedCapsule[0]) return;

	ElapsedTime += DeltaTime;

	switch (CapsuleState)
	{
	case ECapsuleState::Rising:
	{
		float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.f, 1.0f);		
		for (int i = 0; i < 4; i++)
		{
			FVector NewLocation = FMath::Lerp(CapsuleStartLocation[i], CapsuleEndLocation[i], Alpha);
			SpawnedCapsule[i]->SetActorLocation(NewLocation);
		}

		if (Alpha >= 1.0f)
		{
			SpawnEnemy();

			CapsuleState = ECapsuleState::Holding;
			ElapsedTime = 0.0f;
		}
		break;
	}
	case ECapsuleState::Holding:
	{
		if (ElapsedTime >= 5.0f)
		{
			CapsuleState = ECapsuleState::Falling;
			ElapsedTime = 0.0f;
		}
		break;
	}
	case ECapsuleState::Falling:
	{
		float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.f, 1.0f);
		
		for (int i = 0; i < 4; i++)
		{
			FVector NewLocation = FMath::Lerp(CapsuleEndLocation[i], CapsuleStartLocation[i], Alpha);
			SpawnedCapsule[i]->SetActorLocation(NewLocation);

			if (Alpha >= 1.0f && CapsuleState != ECapsuleState::None)
			{
				for (auto Capsule : SpawnedCapsule)
				{
					if (Capsule)
					{
						Capsule->SetActorHiddenInGame(true);
						Capsule->SetActorEnableCollision(false);
						Capsule->SetActorTickEnabled(false);
						Capsule->Destroy();
					}
				}

				CapsuleState = ECapsuleState::None;
				End();
			}
		}
		break;
	}
	default:
		break;
	}
}

void UBoss5SpawnEnemyAttack::Execute()
{
	InitializeArray();
	SpawnCapsule();
}

void UBoss5SpawnEnemyAttack::InitializeArray()
{
	SpawnedCapsuleLocation.Reset();
	CapsuleStartLocation.Reset();
	CapsuleEndLocation.Reset();
	SpawnedEnemyRotation.Reset();
	SpawnedCapsule.Reset();
}

void UBoss5SpawnEnemyAttack::SpawnCapsule()
{
	if (!OwnerCharacter) return;

	AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
	if (!AIController) return;

	BBComp = AIController->GetBlackboardComponent();
	if (!BBComp) return;

	const FName TargetActorKey("TargetActor");
	const FName MeleeLeftKey("MeleeLeftSpawnLocation");
	const FName MeleeRightKey("MeleeRightSpawnLocation");
	const FName RangedLeftKey("RangedLeftSpawnLocation");
	const FName RangedRightKey("RangedRightSpawnLocation");

	FVector MeleeLeftSpawnLocation = BBComp->GetValueAsVector(MeleeLeftKey);
	FVector MeleeRightSpawnLocation = BBComp->GetValueAsVector(MeleeRightKey);
	FVector RangedLeftSpawnLocation = BBComp->GetValueAsVector(RangedLeftKey);
	FVector RangedRightSpawnLocation = BBComp->GetValueAsVector(RangedRightKey);

	SpawnedCapsuleLocation.Add(MeleeLeftSpawnLocation);
	SpawnedCapsuleLocation.Add(MeleeRightSpawnLocation);
	SpawnedCapsuleLocation.Add(RangedLeftSpawnLocation);
	SpawnedCapsuleLocation.Add(RangedRightSpawnLocation);

	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey));
	if (!TargetActor) return;

	UClass* Capsule = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/Blueprints/Boss/Boss5/Capsule/BP_EnemyCapsule.BP_EnemyCapsule_C"));

	if (Capsule)
	{
		for (int i = 0; i < 4; i++)
		{
			FVector StartLocation = SpawnedCapsuleLocation[i] + FVector(0, 0, -240.f);
			TargetLocation = TargetActor->GetActorLocation();
			SpawnedEnemyRotation.Add((TargetLocation - SpawnedCapsuleLocation[i]).Rotation());
			SpawnedCapsule.Add(GetWorld()->SpawnActor<AActor>(Capsule, StartLocation, SpawnedEnemyRotation[i]));

			//	SpawnedCapsule = GetWorld()->SpawnActor<AActor>(Capsule, StartLocation, FRotator(0.f, -180.f, 0.f));

			if (SpawnedCapsule[i])
			{
				CapsuleStartLocation.Add(StartLocation);
				CapsuleEndLocation.Add(SpawnedCapsuleLocation[i]);
				ElapsedTime = 0.0f;
				CapsuleState = ECapsuleState::Rising;
			}
		}
	}
	else
	{
		Debug::PrintError(TEXT("UBoss5SpawnEnemyAttack::Capsule1 Asset's location is not assigned"));
	}
}

void UBoss5SpawnEnemyAttack::SpawnEnemy()
{
	UClass* MeleeAI = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/Dev/JCH/NomalMonster/BP_MinionAI.BP_MinionAI_C"));
	if (MeleeAI)
	{
		for (int i = 0; i < 2; i++)
		{
			SpawnedEnemy.Add(GetWorld()->SpawnActor<AActor>(MeleeAI, SpawnedCapsuleLocation[i], SpawnedEnemyRotation[i]));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UBoss5SpawnEnemyAttack::BP_MinionAI Asset's location is not assigned"));
	}

	UClass* RangedAI = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/Dev/JCH/NomalMonster/BP_GunMinionAI.BP_GunMinionAI_C"));
	if (RangedAI)
	{
		//FVector ChosenSpawnLocation = FMath::RandBool() ? RangedLeftSpawnLocation : RangedRightSpawnLocation;
		//FRotator RangedRot = (TargetLocation - ChosenSpawnLocation).Rotation();

		for (int i = 0; i < 2; i++)
		{
			SpawnedEnemy.Add(GetWorld()->SpawnActor<AActor>(RangedAI, SpawnedCapsuleLocation[i + 2], SpawnedEnemyRotation[i + 2]));
		}
	}
	else
	{
		Debug::PrintError(TEXT("UBoss5SpawnEnemyAttack::BP_GunMinionAI Asset's location is not assigned"));
	}
}

void UBoss5SpawnEnemyAttack::End()
{
	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}