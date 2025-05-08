#include "ItemInventory/BaseItem.h"
#include "GameplayTagContainer.h"
#include "Util/DebugHelper.h"


ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	this->GetItemData(); 
	this->GetItemGameplayTag();
}

FItemData* ABaseItem::GetItemData() const
{
	if (ItemDataTable && ItemDataRowName!= NAME_None)
	{
		Debug::Print(TEXT("Row Found"));
		return ItemDataTable->FindRow<FItemData>(ItemDataRowName, TEXT(""));
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
	const FItemData* Data = GetItemData();
	if (Data)
	{
		Debug::Print(TEXT("return gameplaytag"));
		//return FGameplayTag::RequestGameplayTag(Data->ItemTag);
	}
	return FGameplayTag();
}