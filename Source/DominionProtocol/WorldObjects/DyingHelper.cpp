// Fill out your copyright notice in the Description page of Project Settings.


#include "DyingHelper.h"
#include "ItemInventory/ItemDropped.h"
#include "WorldObjects/DialogueManager.h"

#include "Util/DebugHelper.h"


ADyingHelper::ADyingHelper()
{
	PrimaryActorTick.bCanEverTick = false;

	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	Hair->SetupAttachment(GetMesh());
	
	GetMesh()->bReceivesDecals = false;
}

void ADyingHelper::BeginPlay()
{
	Super::BeginPlay();

	DialogueManager = NewObject<UDialogueManager>(this);
}

void ADyingHelper::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DialogueManager == nullptr;
	Super::EndPlay(EndPlayReason);
}

void ADyingHelper::Interact_Implementation(AActor* Interactor)
{
	//DialogueManager
}

FText ADyingHelper::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("대화하기"));
}

void ADyingHelper::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ADyingHelper::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
}

void ADyingHelper::Die()
{
	// 아이템 드랍
	if (IsValid(ItemDroppedClass.Get()))
	{
		GetWorld()->SpawnActor<AItemDropped>(ItemDroppedClass.Get(), GetActorLocation(), GetActorRotation());
	}

	GetWorldTimerManager().SetTimer(
		DieTimerHandle,
		[this]()
		{
			Destroy();
		},
		3.f,
		false);
}
