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

void UBoss5SpawnEnemyAttack::Execute()
{
	if (!OwnerCharacter) return;

	AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController());
	if (!AIController) return;

	UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	if (!BBComp) return;

	const FName TargetActorKey("TargetActor");
	const FName MeleeLeftKey("MeleeLeftSpawnLocation");
	const FName MeleeRightKey("MeleeRightSpawnLocation");
	const FName RangedLeftKey("RangedLeftSpawnLocation");
	const FName RangedRightKey("RangedRightSpawnLocation");

	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(TargetActorKey));
	if (!TargetActor) return;
	FVector TargetLocation = TargetActor->GetActorLocation();

	FVector MeleeLeftSpawnLocation = BBComp->GetValueAsVector(MeleeLeftKey);
	FVector MeleeRightSpawnLocation = BBComp->GetValueAsVector(MeleeRightKey);
	FVector RangedLeftSpawnLocation = BBComp->GetValueAsVector(RangedLeftKey);
	FVector RangedRightSpawnLocation = BBComp->GetValueAsVector(RangedRightKey);

	UClass* MeleeAI = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/Dev/JCH/NomalMonster/BP_MinionAI.BP_MinionAI_C"));
	if (MeleeAI)
	{
		FRotator MeleeLeftRot = (TargetLocation - MeleeLeftSpawnLocation).Rotation();
		FRotator MeleeRightRot = (TargetLocation - MeleeRightSpawnLocation).Rotation();

		AActor* SpawnedMeleeAI1 = GetWorld()->SpawnActor<AActor>(MeleeAI, MeleeLeftSpawnLocation, MeleeLeftRot);
		AActor* SpawnedMeleeAI2 = GetWorld()->SpawnActor<AActor>(MeleeAI, MeleeRightSpawnLocation, MeleeRightRot);
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

		FRotator RangeLeftRot = (TargetLocation - RangedLeftSpawnLocation).Rotation();
		FRotator RangeRightRot = (TargetLocation - RangedRightSpawnLocation).Rotation();

		AActor* SpawnedRangeAI1 = GetWorld()->SpawnActor<AActor>(RangedAI, RangedLeftSpawnLocation, RangeLeftRot);
		AActor* SpawnedRangeAI2 = GetWorld()->SpawnActor<AActor>(RangedAI, RangedRightSpawnLocation, RangeRightRot);
	}
	else
	{
		Debug::PrintError(TEXT("UBoss5SpawnEnemyAttack::BP_GunMinionAI Asset's location is not assigned"));
	}

	USkillComponent* SkillComponent = OwnerCharacter->FindComponentByClass<USkillComponent>();

	check(SkillComponent);
	SkillComponent->EndSkill();
}

//TArray<FVector> SpawnLocations;
//SpawnLocations.Add(BBComp->GetValueAsVector(RangedLeftKey));
//SpawnLocations.Add(BBComp->GetValueAsVector(RangedRightKey));
//
//// 랜덤 인덱스 선택
//int32 RandomIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
//FVector ChosenSpawnLocation = SpawnLocations[RandomIndex];