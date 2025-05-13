#include "ItemInventory/ElixirConsumable.h"
#include "DominionProtocol/Components/StatusComponent/StatusComponent.h"
#include "DominionProtocolCharacter.h"
#include "Util/DebugHelper.h"

AElixirConsumable::AElixirConsumable()
{
}

void AElixirConsumable::ApplyElixir(ADominionProtocolCharacter* TargetCharacter)
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
