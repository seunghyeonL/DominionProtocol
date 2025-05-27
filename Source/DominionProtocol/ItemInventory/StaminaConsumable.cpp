#include "ItemInventory/StaminaConsumable.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "DominionProtocol/Components/ItemComponent/ItemComponent.h"
#include "Util/DebugHelper.h"

AStaminaConsumable::AStaminaConsumable()
{
}

void AStaminaConsumable::Consume_Implementation(AActor* ConsumedActor)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(ConsumedActor);
	if (TargetCharacter)
	{
		ApplyStaminaPotion(TargetCharacter);

		// 아이템 컴포넌트 찾아서 아이템 제거
		UItemComponent* ItemComp = TargetCharacter->FindComponentByClass<UItemComponent>();
		if (ItemComp)
		{
			ItemComp->RemoveItem(GetItemGameplayTag(), 1);
			Debug::Print(TEXT("스테미너 포션 소비 후 인벤토리에서 제거"));
		}
		else
		{
			Debug::Print(TEXT("스테미너 포션을 소비하려 했으나 대상에게 ItemComponent가 없습니다."));
		}
	}
	else
	{
		Debug::Print(TEXT("포션을 소비하려 했으나 대상이 DomiCharacter가 아닙니다."));
	}
}

FText AStaminaConsumable::GetConsumeMessage_Implementation() const
{
	return FText::FromString(TEXT("키를 눌러 스테미너 포션 소비"));
}

void AStaminaConsumable::ApplyStaminaPotion(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float MaxStamina = StatusComp->GetStat(StatTags::MaxStamina);
			float CurrentStamina = StatusComp->GetStat(StatTags::Stamina);
			float NewStamina = FMath::Clamp(CurrentStamina + RecoveryAmount, 0, MaxStamina);

			StatusComp->SetStamina(NewStamina);
			Debug::Print(TEXT("Stamina Recovered by Potion"));
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found"));
		}
	}
}
