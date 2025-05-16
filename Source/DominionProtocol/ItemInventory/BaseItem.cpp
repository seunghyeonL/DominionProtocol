#include "ItemInventory/BaseItem.h"
#include "Util/GameTagList.h"
#include "Util/DebugHelper.h"
#include "Components/ItemComponent/ItemComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h" 

ABaseItem::ABaseItem()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	//상호작용 볼륨
	InteractionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Overlap);
	InteractionVolume->SetGenerateOverlapEvents(true);

	// 오버랩 이벤트 바인딩
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnOverlapBegin);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnOverlapEnd);
}

void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	GetItemData(); 
	GetItemGameplayTag();
}

//아이템 데이터 Row 반환
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

//상호 작용 함수 따로 사용x
void ABaseItem::InteractItem(APawn* InteractingPawn)
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

//상호 작용 인터페이스
FText ABaseItem::GetInteractMessage_Implementation() const
{
	return FText::FromString(TEXT("F 키를 눌러 아이템을 획득"));
}

//상호 작용 인터페이스
void ABaseItem::Interact_Implementation(AActor* Interactor)
{
	APawn* InteractingPawn = Cast<APawn>(Interactor);
	if (InteractingPawn)
	{
		UItemComponent* InventoryComp = InteractingPawn->FindComponentByClass<UItemComponent>();
		if (InventoryComp)
		{
			const FItemData* Data = GetItemData();
			if (Data)
			{
				// 인벤토리에 이미 아이템이 있는지 확인하고, 최대 수량 초과 여부 확인
				if (InventoryComp->HasItem(Data->ItemTag) && InventoryComp->GetItemQuantity(Data->ItemTag) >= Data->MaxItemQuantity)
				{
					Debug::Print(TEXT("인벤토리에 최대 수량만큼 아이템이 존재합니다."));
					return; // 더 이상 아이템을 줍지 않음
				}
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

void ABaseItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->AddInteractableActor(this);
		Debug::Print(TEXT("아이템과 오버랩 시작: InteractableObject 설정"));
	}
}

void ABaseItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADomiCharacter* PlayerCharacter = Cast<ADomiCharacter>(OtherActor);
	if (PlayerCharacter && PlayerCharacter->IsLocallyControlled())
	{
		PlayerCharacter->RemoveInteractableActor(this);
		Debug::Print(TEXT("아이템과 오버랩 종료: InteractableObject 해제"));
	}
}