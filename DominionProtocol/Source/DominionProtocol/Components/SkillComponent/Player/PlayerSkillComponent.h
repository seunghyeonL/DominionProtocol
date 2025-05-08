#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/SkillComponentBase.h"
#include "PlayerSkillComponent.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UPlayerSkillComponent : public USkillComponentBase
{
	GENERATED_BODY()
	
public:

	UPlayerSkillComponent();

	virtual void ExecuteSkill(FGameplayTag SkillGroupTag) override;
};