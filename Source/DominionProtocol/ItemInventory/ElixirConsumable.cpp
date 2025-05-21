#include "ItemInventory/ElixirConsumable.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "DominionProtocol/Components/ItemComponent/ItemComponent.h"
#include "Util/DebugHelper.h"

AElixirConsumable::AElixirConsumable()
{
}

void AElixirConsumable::ApplyElixir(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			for (const FGameplayTag& EffectTagToRemove : RemoveableStatusEffects)
			{
				StatusComp->DeactivateStatusEffect(EffectTagToRemove);
				Debug::Print(TEXT("Status Effect Removed"));
			}
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found"));
		}
	}
}

void AElixirConsumable::Consume_Implementation(AActor* ConsumedActor)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(ConsumedActor);
	if (TargetCharacter)
	{
		ApplyElixir(TargetCharacter);

		// 아이템 컴포넌트 찾아서 아이템 제거
		UItemComponent* ItemComp = TargetCharacter->FindComponentByClass<UItemComponent>();
		if (ItemComp)
		{
			ItemComp->RemoveItem(GetItemGameplayTag(), 1);
			Debug::Print(TEXT("엘릭서 소비 후 인벤토리에서 제거"));
		}
		else
		{
			Debug::Print(TEXT("엘릭서를 소비하려 했으나 대상에게 ItemComponent가 없습니다."));
		}
		// Destroy(); // 더 이상 Consume_Implementation에서 아이템 액터를 바로 제거하지 않습니다.
	}
	else
	{
		Debug::Print(TEXT("엘릭서를 소비하려 했으나 대상이 DomiCharacter가 아닙니다."));
	}
}

FText AElixirConsumable::GetConsumeMessage_Implementation() const
{
	return FText::FromString(TEXT("키를 눌러 엘릭서 소비"));
}