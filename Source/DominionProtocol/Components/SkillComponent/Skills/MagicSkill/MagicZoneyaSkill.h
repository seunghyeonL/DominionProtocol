#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "MagicZoneyaSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UMagicZoneyaSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UMagicZoneyaSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

private:
	void StartZoneya();

	void EndZoneya();

	FTimerHandle ZoneyaTimerHandle;

	float Duration;
};
