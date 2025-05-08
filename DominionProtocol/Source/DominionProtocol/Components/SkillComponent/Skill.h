#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Util/BattleDataTypes.h"
#include "Skill.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API USkill : public UObject
{
	GENERATED_BODY()

public:
	USkill();

	void Excute(ACharacter* Owner) const;

	void AttackTrace(const ACharacter* Owner) const;

	float GetStamina() const;

private:
	float GetFinalAttackData(const float AttackPower) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USoundBase> Sound = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystem> Particle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float DamageCoefficient;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	FAttackData BaseAttackData;
};
