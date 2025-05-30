#include "ItemInventory/ItemDropped.h"
#include "Particles/ParticleSystem.h" 
#include "Components/ItemComponent/ItemComponent.h" 
#include "ItemData.h"
#include "Util/DebugHelper.h"

AItemDropped::AItemDropped()
{
	// VFX 컴포넌트 생성 및 초기화
	AuraVFXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AuraVFX"));
	AuraVFXComponent->SetupAttachment(RootComponent); //스태틱 메시에 attach
	AuraVFXComponent->bAutoActivate = true; //VFX를 재생

	ItemSubclassToAward = ABaseItem::StaticClass();
}

void AItemDropped::BeginPlay()
{
	Super::BeginPlay();
	if (ItemSubclassToAward)
	{
		ABaseItem* DefaultItemObject = ItemSubclassToAward.GetDefaultObject();
		if (DefaultItemObject)
		{
			FGameplayTag TagToSearch = DefaultItemObject->GetItemGameplayTag();
			if (ItemDataTable && TagToSearch.IsValid())
			{
				const FItemData* ItemData = ItemDataTable->FindRow<FItemData>(TagToSearch.GetTagName(), TEXT("AItemDropped::BeginPlay"));
				if (ItemData)
				{
					if (ItemData->ItemStaticMesh)
					{
						StaticMesh->SetStaticMesh(ItemData->ItemStaticMesh);
						Debug::Print(FString::Printf(TEXT("ItemDropped: Static Mesh set to %s from Data Table."), *ItemData->ItemStaticMesh->GetName()));
					}
					else
					{
						Debug::Print(FString::Printf(TEXT("ItemDropped: ItemStaticMesh is null in FItemData for tag '%s'."), *TagToSearch.ToString()), FColor::Yellow);
					}
				}
				else
				{
					Debug::Print(FString::Printf(TEXT("ItemDropped: Failed to find FItemData row for tag '%s' in DataTable."), *TagToSearch.ToString()), FColor::Red);
				}
			}
			else
			{
				Debug::Print(TEXT("ItemDropped: ItemDataTable is null or ItemTag from ItemSubclassToAward is invalid."), FColor::Red);
			}
		}
		else
		{
			Debug::Print(TEXT("ItemDropped: Failed to get default object from ItemSubclassToAward."), FColor::Red);
		}
	}
	else
	{
		Debug::Print(TEXT("ItemDropped: ItemSubclassToAward is not set!"), FColor::Red);
	}

	// AuraVFXComponent에 파티클 시스템이 할당되어 있다면 재생
	if (AuraVFXComponent && AuraVFXComponent->Template)
	{
		AuraVFXComponent->ActivateSystem();
	}
}

void AItemDropped::Interact_Implementation(AActor* Interactor)
{
	APawn* InteractingPawn = Cast<APawn>(Interactor);
	if (InteractingPawn)
	{
		UItemComponent* InventoryComp = InteractingPawn->FindComponentByClass<UItemComponent>();
		if (InventoryComp)
		{
			// ItemSubclassToAward가 유효한지 확인
			if (ItemSubclassToAward)
			{
				ABaseItem* DefaultItemObject = ItemSubclassToAward.GetDefaultObject();
				if (DefaultItemObject)
				{
					const FItemData* Data = DefaultItemObject->GetItemData();
					if (Data)
					{
						// 인벤토리에 이미 아이템이 있는지 확인하고, 최대 수량 초과 여부 확인
						if (InventoryComp->HasItem(Data->ItemTag) && InventoryComp->GetItemQuantity(Data->ItemTag) >= Data->MaxItemQuantity)
						{
							Debug::Print(TEXT("인벤토리에 최대 수량만큼 아이템이 존재합니다."));
							return;
						}

						// 지정된 서브클래스의 아이템 태그를 사용해 아이템 추가
						InventoryComp->AddItem(Data->ItemTag, 1);
						Destroy(); 
					}
					else
					{
						Debug::Print(TEXT("Invalid Item Data from ItemSubclassToAward"));
					}
				}
				else
				{
					Debug::Print(TEXT("Failed to get default object from ItemSubclassToAward"));
				}
			}
			else
			{
				Debug::Print(TEXT("ItemSubclassToAward is not set!"));
			}
		}
	}
}