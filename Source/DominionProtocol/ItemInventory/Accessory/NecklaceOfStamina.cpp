#include "ItemInventory/Accessory/NecklaceOfStamina.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "DominionProtocol/Components/ItemComponent/ItemComponent.h"
#include "Util/DebugHelper.h"

ANecklaceOfStamina::ANecklaceOfStamina()
{
}

void ANecklaceOfStamina::Equip_Implementation(AActor* EquipCharacter)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(EquipCharacter);
	if (TargetCharacter)
	{
		EquipNecklaceOfStamina(TargetCharacter);
	}
}

void ANecklaceOfStamina::UnEquip_Implementation(AActor* UnequipCharacter)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(UnequipCharacter);
	if (TargetCharacter)
	{
		UnEquipNecklaceOfStamina(TargetCharacter);
	}
}

FText ANecklaceOfStamina::GetEquipMessage_Implementation() const
{
	return FText();
}

void ANecklaceOfStamina::EquipNecklaceOfStamina(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float CurMaxStamina = StatusComp->GetStat(StatTags::MaxStamina);
			float NewMaxStamina = CurMaxStamina + MaxStaminaBoostAmount;

			float CurStamina = StatusComp->GetStat(StatTags::Stamina);
			float NewStamina = CurStamina + MaxStaminaBoostAmount;

			StatusComp->SetMaxStamina(NewMaxStamina);
			StatusComp->SetStamina(NewStamina);
			Debug::Print(TEXT("Max Stamina Boosted"));
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found"));
		}
	}
}

void ANecklaceOfStamina::UnEquipNecklaceOfStamina(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float CurMaxStamina = StatusComp->GetStat(StatTags::MaxStamina);
			float NewMaxStamina = CurMaxStamina - MaxStaminaBoostAmount; // 효과 되돌리기

			float CurStamina = StatusComp->GetStat(StatTags::Stamina);
			float NewStamina = FMath::Clamp(CurStamina, 0.f, NewMaxStamina);

			StatusComp->SetMaxStamina(NewMaxStamina);
			StatusComp->SetStamina(NewStamina);
			Debug::Print(TEXT("Max Stamina Deboosted by Necklace of Stamina (Unequipped)"));
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found on character to unequip Necklace"));
		}
	}
}
