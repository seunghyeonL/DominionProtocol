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

void ARingOfHealth::UnEquip_Implementation(AActor* UnequipCharacter) // 새로 추가된 부분
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(UnequipCharacter);
	if (TargetCharacter)
	{
		UnEquipRingOfHealth(TargetCharacter);
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

void ARingOfHealth::UnEquipRingOfHealth(ADomiCharacter* TargetCharacter) 
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float CurMaxHealth = StatusComp->GetStat(StatTags::MaxHealth);
			float NewMaxHealth = CurMaxHealth - MaxHealthBoostAmount; // 효과 되돌리기

			StatusComp->SetStat(StatTags::MaxHealth, NewMaxHealth);
			Debug::Print(TEXT("Max Health Deboosted by Ring of Health (Unequipped)"));
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found on character to unequip ring"));
		}
	}
}