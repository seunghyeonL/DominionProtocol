#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillComponentUser.generated.h"

UINTERFACE(MinimalAPI)
class USkillComponentUser : public UInterface
{
	GENERATED_BODY()
};

class DOMINIONPROTOCOL_API ISkillComponentUser
{
	GENERATED_BODY()

public:
	virtual void InitializeSkillComponent() = 0;
	virtual void ExecuteSkill(FGameplayTag SkillGroupTag) = 0;

};