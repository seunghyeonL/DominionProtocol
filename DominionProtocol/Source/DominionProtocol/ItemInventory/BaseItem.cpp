#include "ItemInventory/BaseItem.h"
#include "GameplayTagContainer.h"
#include "Util/DebugHelper.h"


ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	this->GetItemData();
}

FItemData* ABaseItem::GetItemData() const
{
	if (!ItemDataTable)
	{
		Debug::Print(TEXT("DataTable Not Found"));
		return nullptr;
	}
	TArray<FItemData*> AllRows;
	static const FString ContextString(TEXT("ItemDataContext"));
	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty())
	{
		Debug::Print(TEXT("Row Empty"));
		return nullptr;
	}
	Debug::Print(TEXT("Row Found"));
	return ItemDataTable->FindRow<FItemData>(ItemDataRowName, TEXT(""));
}