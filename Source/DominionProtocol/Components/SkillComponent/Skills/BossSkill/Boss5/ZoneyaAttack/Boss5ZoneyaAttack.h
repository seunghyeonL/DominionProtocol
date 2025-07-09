#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "Boss5ZoneyaAttack.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API UBoss5ZoneyaAttack : public UBaseSkill
{
	GENERATED_BODY()
	
public:
	UBoss5ZoneyaAttack();

	virtual void Execute() override;

	void EndZoneya();

	FTimerHandle ZoneyaTimerHandle;

	UPROPERTY()
	TObjectPtr<ACharacter> TargetCharacter;

	UPROPERTY()
	TObjectPtr<APlayerController> PC;
};
