#include "ItemInventory/AddMaxPotionConsumable.h"
#include "DominionProtocol/Components/ItemComponent/ItemComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "Util/DebugHelper.h"

AAddMaxPotionConsumable::AAddMaxPotionConsumable()
{
}

void AAddMaxPotionConsumable::Consume_Implementation(AActor* ConsumedActor)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(ConsumedActor);
	if (TargetCharacter)
	{
		UItemComponent* ItemComp = TargetCharacter->FindComponentByClass<UItemComponent>();
		if (ItemComp)
		{
			// UItemComponent의 ApplyPotionBoost 함수 호출
			ItemComp->ApplyPotionBoost();

			// 이 MaxPotionBoostConsumable 아이템을 인벤토리에서 제거
			ItemComp->RemoveItem(GetItemGameplayTag(), 1);
			Debug::Print(TEXT("Max Potion Boost consumed and potions upgraded!"));
		}
		else
		{
			Debug::Print(TEXT("Failed to find ItemComponent on the consuming character."));
		}
	}
	else
	{
		Debug::Print(TEXT("Max Potion Boost was consumed by a non-DomiCharacter actor."));
	}
}

FText AAddMaxPotionConsumable::GetConsumeMessage_Implementation() const
{
	return FText::FromString(TEXT("키를 눌러 포션 부스트 소비"));
}
