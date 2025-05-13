#include "ItemInventory/PotionConsumable.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocolCharacter.h"
#include "Util/DebugHelper.h"

APotionConsumable::APotionConsumable()
{
}

void APotionConsumable::ApplyPotion(ADominionProtocolCharacter* TargetCharacter)
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