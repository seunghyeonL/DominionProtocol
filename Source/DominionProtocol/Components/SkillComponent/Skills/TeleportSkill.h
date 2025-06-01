#pragma once

#include "CoreMinimal.h"
#include "Components/SkillComponent/Skills/BaseSkill.h"
#include "TeleportSkill.generated.h"

class ADomiCharacter;
class UStaticMeshComponent;

constexpr float BackwardOffsetDistance = 100.f;
constexpr float DownTraceLength = 1000.f;

UCLASS()
class DOMINIONPROTOCOL_API UTeleportSkill : public UBaseSkill
{
	GENERATED_BODY()

public:
	UTeleportSkill();

	virtual void Initialize(ACharacter* OwnerCharacter) override;

	virtual void Execute() override;

private:
	void Move();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	ADomiCharacter* DomiChar;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StartPoint;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BeginTrace;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> MovePoint;
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> Aura;
	UPROPERTY()
	TObjectPtr<UParticleSystemComponent> CantAura;

	bool IsTeleport;
	bool CanTeleport;
};
