#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "TeleportSkill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UTeleportSkill : public UBaseSkill
{
	GENERATED_BODY()

public:
	UTeleportSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

private:
	void Start();

	void End();

	FTimerHandle TeleportTimerHandle;

	float Distance;
};
