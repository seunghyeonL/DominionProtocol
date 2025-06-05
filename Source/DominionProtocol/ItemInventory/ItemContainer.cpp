// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInventory/ItemContainer.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "ItemInventory/BaseItem.h"
#include "NiagaraComponent.h"
#include "DomiFramework/WorldActorManage/ActorStateComponent.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "Util/GameTagList.h"
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

	ActorStateComponent = CreateDefaultSubobject<UActorStateComponent>(TEXT("ActorStateComponent"));
	ActorStateComponent->SetGameplayTag(WorldActorTags::ItemContainer);

	//콜리전 오버랩 채널 설정
	ContainerShellMesh->SetCollisionResponseToAllChannels(ECR_Block); // 기본은 모든 채널에 대해 Block
	ContainerShellMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // <-- 캐릭터 공격 채널에 대해 Overlap

	DustVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DustVFX"));
	DustVFXComponent->SetupAttachment(RootComponent); //스태틱 메시에 attach
	DustVFXComponent->bAutoActivate = false; 

	DustVFXComponent->SetRelativeLocation(FVector::ZeroVector);
	DustVFXComponent->SetRelativeRotation(FRotator::ZeroRotator);
	DustVFXComponent->SetRelativeScale3D(FVector(1.0f));

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
			if (CurrentHealth > 0)
			{
				PlayHitSound();
			}
			PlayShakeAnimation();

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
	if (DustVFXComponent && DustNiagaraSystem)
	{
		DustVFXComponent->SetAsset(DustNiagaraSystem);
		DustVFXComponent->Activate(true);
	}
	else
	{
		Debug::Print(TEXT("ItemDropped: DustVFXComponent or DustNiagaraSystem is null. Niagara effect will not play."), FColor::Yellow);
	}
	PlayDestructionSound();
	if (!ActorStateComponent->GetActorData().bIsItemCollected)
	{
		SpawnRandomItems();
	}
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AItemContainer::DestroyContainerActor, DestroyDelayAfterBreak, false);

	SetCanBeDamaged(false); // 더 이상 데미지를 받지 않도록 설정
}

void AItemContainer::DestroyContainerActor()
{
	Destroy();
}

void AItemContainer::PlayDestructionSound()
{
	if (DestructionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation());
	}
	else
	{
		Debug::Print(TEXT("DestructionSound is null. Cannot play Destruction sound."), FColor::Yellow);
	}
}

void AItemContainer::PlayHitSound()
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	else
	{
		Debug::Print(TEXT("HitSound is null. Cannot play Hit sound."), FColor::Yellow);
	}
}

void AItemContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemContainer::SpawnRandomItems()
{
	UWorld* World = GetWorld();
	if (World == nullptr || ItemsToSpawn.Num() == 0)
	{
		// 스폰할 아이템이 없으면 스폰하지 않음
		return;
	}

	// 스폰할 아이템 배열에서 무작위로 클래스 선택
	int32 RandomItemIndex = FMath::RandRange(0, ItemsToSpawn.Num() - 1);
	TSubclassOf<ABaseItem> ItemClassToSpawn = ItemsToSpawn[RandomItemIndex];

	if (ItemClassToSpawn)
	{
		FVector SpawnLocation = GetActorLocation();
		SpawnLocation.X += FMath::RandRange(-50.0f, 50.0f);
		SpawnLocation.Y += FMath::RandRange(-50.0f, 50.0f);
		SpawnLocation.Z += 20.0f; //높이 조정

		FRotator SpawnRotation = FRotator::ZeroRotator;

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// 아이템 스폰
		ABaseItem* SpawnedItem = World->SpawnActor<ABaseItem>(ItemClassToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
		if (SpawnedItem)
		{
			ActorStateComponent->SwitchStateAndUpdateInstance(WorldActorTags::ItemContainer);
			Debug::Print(FString::Printf(TEXT("아이템 스폰됨: %s"), *SpawnedItem->GetName()));
		}
	}
}