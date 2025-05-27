#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillComponentUser.generated.h"

class USkillComponent;
struct FGameplayTag;

UINTERFACE()
class USkillComponentUser : public UInterface
{
	GENERATED_BODY()
};

class DOMINIONPROTOCOL_API ISkillComponentUser
{
	GENERATED_BODY()

public:
	virtual USkillComponent* GetSkillComponent() const = 0;
	virtual void InitializeSkillComponent() = 0;
	virtual void ExecuteSkill(FGameplayTag SkillGroupTag) = 0;
	
};