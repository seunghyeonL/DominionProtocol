#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "InfiniteStaminaSkill.generated.h"

class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API UInfiniteStaminaSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UInfiniteStaminaSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

private:
	void Start();

	void End();

	FTimerHandle InfiniteTimerHandle;

	float Duration;

	TObjectPtr<UStatusComponent> StatusComponent;
};
