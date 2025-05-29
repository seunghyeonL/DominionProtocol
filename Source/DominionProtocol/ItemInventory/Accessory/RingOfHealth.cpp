#include "ItemInventory/Accessory/RingOfHealth.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "DominionProtocol/Components/ItemComponent/ItemComponent.h"
#include "Util/DebugHelper.h"

ARingOfHealth::ARingOfHealth()
{
}

void ARingOfHealth::Equip_Implementation(AActor* EquipCharacter)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(EquipCharacter);
	if (TargetCharacter)
	{
		EquipRingOfHealth(TargetCharacter);
	}
}

FText ARingOfHealth::GetEquipMessage_Implementation() const
{
	return FText();
}

void ARingOfHealth::EquipRingOfHealth(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float CurMaxHealth = StatusComp->GetStat(StatTags::MaxHealth);
			float NewMaxHealth = CurMaxHealth + MaxHealthBoostAmount;
			
			StatusComp->SetStat(StatTags::MaxHealth, NewMaxHealth);
			Debug::Print(TEXT("Max Health Boosted"));
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found"));
		}
	}
}
