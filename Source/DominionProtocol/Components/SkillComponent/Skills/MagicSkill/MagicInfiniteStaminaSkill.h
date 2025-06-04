#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "MagicInfiniteStaminaSkill.generated.h"

class UStatusComponent;

UCLASS()
class DOMINIONPROTOCOL_API UMagicInfiniteStaminaSkill : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UMagicInfiniteStaminaSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

private:
	void Start();

	void End();

	FTimerHandle InfiniteTimerHandle;

	float Duration;

	TObjectPtr<UStatusComponent> StatusComponent;
};
