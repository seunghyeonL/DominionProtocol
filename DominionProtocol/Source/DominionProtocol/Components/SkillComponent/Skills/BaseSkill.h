#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Util/BattleDataTypes.h"
#include "BaseSkill.generated.h"

UCLASS(Blueprintable)
class DOMINIONPROTOCOL_API UBaseSkill : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseSkill();

	void Excute(ACharacter* Owner);

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
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float AttackForwardOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	float DamageCoefficient;

	FAttackData BaseAttackData;
};
