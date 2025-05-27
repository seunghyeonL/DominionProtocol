#include "ItemInventory/PotionConsumable.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "DominionProtocol/Components/ItemComponent/ItemComponent.h"
#include "Util/DebugHelper.h"

APotionConsumable::APotionConsumable()
{
}

void APotionConsumable::ApplyPotion(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float MaxHealth = StatusComp->GetStat(StatTags::MaxHealth);
			float CurrentHealth = StatusComp->GetStat(StatTags::Health);
			float NewHealth = FMath::Clamp(CurrentHealth + RecoveryAmount,0,MaxHealth);

			StatusComp->SetHealth(NewHealth);
			Debug::Print(TEXT("Recovered by Potion"));
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found"));
		}
	}
}

void APotionConsumable::Consume_Implementation(AActor* ConsumedActor)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(ConsumedActor);
	if (TargetCharacter)
	{
		ApplyPotion(TargetCharacter);

		// 아이템 컴포넌트 찾아서 아이템 제거
		UItemComponent* ItemComp = TargetCharacter->FindComponentByClass<UItemComponent>();
		if (ItemComp)
		{
			ItemComp->RemoveItem(GetItemGameplayTag(), 1);
			Debug::Print(TEXT("포션 소비 후 인벤토리에서 제거"));
		}
		else
		{
			Debug::Print(TEXT("포션을 소비하려 했으나 대상에게 ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::Print(TEXT("포션을 소비하려 했으나 대상이 DomiCharacter가 아닙니다."));
	}
}

FText APotionConsumable::GetConsumeMessage_Implementation() const
{
	return FText::FromString(TEXT("키를 눌러 포션 소비"));
}