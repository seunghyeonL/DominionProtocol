#include "ItemInventory/BaseItem.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "ItemInventory/ItemComponent.h"

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Overlap);
	InteractionVolume->SetGenerateOverlapEvents(true);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	GetItemData(); 
	GetItemGameplayTag();
}

FItemData* ABaseItem::GetItemData() const
{
	if (ItemDataTable && ItemTag.IsValid())
	{
		Debug::Print(TEXT("Row Found"));
		FString Msg = ItemTag.ToString();
		Debug::Print(Msg);
		return ItemDataTable->FindRow<FItemData>(ItemTag.GetTagName(), TEXT(""));
	}
	else
	{
		Debug::Print(TEXT("Row or Table NotFound"));
		return nullptr;
	}
}
//아이템 태그 반환
FGameplayTag ABaseItem::GetItemGameplayTag() const
{
	FString Msg = ItemTag.ToString();
	Debug::Print(Msg);
	return ItemTag.IsValid() ? ItemTag : ItemTags::ItemBase;
}

//상호작용
void ABaseItem::Interact(APawn* InteractingPawn)
{
	if (InteractingPawn)
	{
		UItemComponent* InventoryComp = InteractingPawn->FindComponentByClass<UItemComponent>();
		if (InventoryComp)
		{
			const FItemData* Data = GetItemData();
			if (Data)
			{
				//아이템 추가 후 액터 제거
				InventoryComp->AddItem(Data->ItemTag, 1);
				Destroy();
			}
			else
			{
				Debug::Print(TEXT("Invalid Item Data"));
			}
		}
	}
}