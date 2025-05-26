#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "ZoneyaSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UZoneyaSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UZoneyaSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

private:
	void StartZoneya();

	void EndZoneya();

	FTimerHandle ZoneyaTimerHandle;

	float Duration;
};
