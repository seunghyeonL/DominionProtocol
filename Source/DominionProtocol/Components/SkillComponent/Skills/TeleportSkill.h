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

    // 유효한 텔레포트 위치를 찾는 함수
    //FVector FindValidTeleportLocation(const FVector& StartLocation, const FVector& Direction, float MaxDistance);

    // 착지 위치를 찾는 함수
    //FVector FindLandingLocation(const FVector& StartLocation, const FVector& Direction, float RemainingDistance);

    // 지면 높이에 맞게 조정하는 함수
    //FVector AdjustToGround(const FVector& Location);

	FTimerHandle TeleportTimerHandle;

	float Distance;
};
