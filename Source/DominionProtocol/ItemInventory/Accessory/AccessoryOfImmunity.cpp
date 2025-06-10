#include "ItemInventory/Accessory/AccessoryOfImmunity.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocol/Player/Characters/DomiCharacter.h"
#include "Util/DebugHelper.h"

AAccessoryOfImmunity::AAccessoryOfImmunity()
{
}

void AAccessoryOfImmunity::Equip_Implementation(AActor* EquipCharacter)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(EquipCharacter);
	if (TargetCharacter)
	{
		EquipAccessoryOfImmunity(TargetCharacter);
	}
}

void AAccessoryOfImmunity::UnEquip_Implementation(AActor* UnequipCharacter)
{
	ADomiCharacter* TargetCharacter = Cast<ADomiCharacter>(UnequipCharacter);
	if (TargetCharacter)
	{
		UnEquipAccessoryOfImmunity(TargetCharacter);
	}
}

FText AAccessoryOfImmunity::GetEquipMessage_Implementation() const
{
	return FText();
}

void AAccessoryOfImmunity::EquipAccessoryOfImmunity(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			if (ImmuneStatusEffectTag.IsValid())
			{
				StatusComp->AddImmuneStatusEffect(ImmuneStatusEffectTag);
				Debug::Print(FString::Printf(TEXT("AccessoryOfImmunity: Added immunity to %s."), *ImmuneStatusEffectTag.ToString()));
			}
			else
			{
				Debug::PrintError(TEXT("AccessoryOfImmunity: ImmuneStatusEffectTag is not set on the Accessory!"));
			}
		}
		else
		{
			Debug::Print(TEXT("AccessoryOfImmunity: No Status Component Found on character to equip accessory."));
		}
	}
}

void AAccessoryOfImmunity::UnEquipAccessoryOfImmunity(ADomiCharacter* TargetCharacter)
{
	if (TargetCharacter)
	{
		UStatusComponent* StatusComp = TargetCharacter->FindComponentByClass<UStatusComponent>();
		if (StatusComp)
		{
			if (ImmuneStatusEffectTag.IsValid())
			{
				StatusComp->RemoveImmuneStatusEffect(ImmuneStatusEffectTag);
				Debug::Print(FString::Printf(TEXT("AccessoryOfImmunity: Removed immunity to %s."), *ImmuneStatusEffectTag.ToString()));
			}
			else
			{
				Debug::PrintError(TEXT("AccessoryOfImmunity: ImmuneStatusEffectTag is not set on the Accessory during unequip!"));
			}
		}
		else
		{
			Debug::Print(TEXT("AccessoryOfImmunity: No Status Component Found on character to unequip accessory."));
		}
	}
}
