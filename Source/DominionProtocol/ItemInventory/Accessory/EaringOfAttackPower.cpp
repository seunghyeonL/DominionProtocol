#include "ItemInventory/Accessory/EaringOfAttackPower.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "DominionProtocol/Components/ItemComponent/ItemComponent.h"
#include "Util/DebugHelper.h"

AEaringOfAttackPower::AEaringOfAttackPower()
{
}

void AEaringOfAttackPower::Equip_Implementation(AActor* EquipCharacter)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(EquipCharacter);
	if (TargetCharacter)
	{
		EquipEaringOfAttackPower(TargetCharacter);
	}
}

void AEaringOfAttackPower::UnEquip_Implementation(AActor* UnequipCharacter)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(UnequipCharacter);
	if (TargetCharacter)
	{
		UnEquipEaringOfAttackPower(TargetCharacter);
	}
}

FText AEaringOfAttackPower::GetEquipMessage_Implementation() const
{
    return FText();
}

void AEaringOfAttackPower::EquipEaringOfAttackPower(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float CurAttackPower = StatusComp->GetStat(StatTags::AttackPower);
			float NewAttackPower = CurAttackPower + AttackPowerBoostAmount;

			StatusComp->SetAttackPower(NewAttackPower);
			Debug::Print(TEXT("Attack Power Boosted"));
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found"));
		}
	}
}

void AEaringOfAttackPower::UnEquipEaringOfAttackPower(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			float CurAttackPower = StatusComp->GetStat(StatTags::AttackPower);
			float NewAttackPower = CurAttackPower - AttackPowerBoostAmount; // 효과 되돌리기

			StatusComp->SetMaxStamina(NewAttackPower);
			Debug::Print(TEXT("Attack Power Deboosted by EaringOfAttackPower (Unequipped)"));
		}
		else
		{
			Debug::Print(TEXT("No Status Component Found on character to unequip Earing"));
		}
	}
}
