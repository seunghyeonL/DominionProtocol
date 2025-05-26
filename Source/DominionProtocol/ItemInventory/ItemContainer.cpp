// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventory/ItemContainer.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "Util/BattleDataTypes.h"
#include "Util/DebugHelper.h"

// Sets default values
AItemContainer::AItemContainer()
{
	PrimaryActorTick.bCanEverTick = false;

	ContainerShellMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContainerShellMesh"));
	SetRootComponent(ContainerShellMesh);

	ContainerShellMesh->SetCollisionProfileName(TEXT("Custom")); // 사용자 정의 콜리전으로 변경
	ContainerShellMesh->SetGenerateOverlapEvents(true); // 오버랩 이벤트를 생성하도록 설정

	//콜리전 오버랩 채널 설정
	ContainerShellMesh->SetCollisionResponseToAllChannels(ECR_Block); // 기본은 모든 채널에 대해 Block
	ContainerShellMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // <-- 캐릭터 공격 채널에 대해 Overlap

	CurrentHealth = MaxHealth;

	SetCanBeDamaged(true);
}

// Called when the game starts or when spawned
void AItemContainer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemContainer::OnAttacked_Implementation(const FAttackData& AttackData)
{
	// 공격자가 ADomiCharacter 타입인지 확인하여 캐릭터의 공격에만 반응
	if (Cast<ADomiCharacter>(AttackData.Instigator))
	{
		if (CurrentHealth > 0)
		{
			CurrentHealth -= AttackData.Damage;
			Debug::Print(FString::Printf(TEXT("컨테이너 현재 체력: %.2f"), CurrentHealth));

			if (CurrentHealth <= 0)
			{
				OnHealthZeroed();
			}
		}
	}
}

void AItemContainer::OnHealthZeroed()
{
	if (IsValid(ContainerShellMesh))
	{
		ContainerShellMesh->SetVisibility(false); // 껍데기 메시 숨기기
		ContainerShellMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 껍데기 메시 콜리전 비활성화
	}
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AItemContainer::DestroyContainerActor, DestroyDelayAfterBreak, false);

	SetCanBeDamaged(false); // 더 이상 데미지를 받지 않도록 설정
}

void AItemContainer::DestroyContainerActor()
{
	Destroy();
}

void AItemContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}