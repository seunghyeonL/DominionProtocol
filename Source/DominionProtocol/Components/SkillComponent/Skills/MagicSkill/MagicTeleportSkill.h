#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "MagicTeleportSkill.generated.h"

class APortal;

constexpr float ForwardOffsetDistance = 10.f;
constexpr float DownTraceDistance = 5000.f;
constexpr float MaxTeleportDistance = 1200.f;
constexpr float MaxSlopeAngle = 45.f;

UCLASS()
class DOMINIONPROTOCOL_API UMagicTeleportSkill : public UBaseSkill
{
	GENERATED_BODY()

public:
	UMagicTeleportSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

	FORCEINLINE void SetReadyToTeleport(bool ReadyFlag) { bReadyToTeleport = ReadyFlag; }
	FORCEINLINE void SetCanTeleport(bool CanFlag) { bCanTeleport = CanFlag; }

	UPROPERTY()
	TObjectPtr<APortal> Portal;

private:
	virtual void Tick(float DeltaTime) override;

	void Move();

	bool IsValidAngle(const FVector& StartLocation, const FVector& TargetLocation);
	bool IsValidDistance(const FVector& StartLocation);

	void ActivateBlue();
	void ActivateRed();

	FVector OwnerLocation;

	bool bReadyToTeleport;
	bool bCanTeleport;
};