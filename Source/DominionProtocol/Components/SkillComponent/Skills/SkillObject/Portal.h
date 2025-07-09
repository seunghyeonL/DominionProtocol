#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Portal.generated.h"

class UMagicTeleportSkill;

UCLASS()
class DOMINIONPROTOCOL_API APortal : public AActor
{
	GENERATED_BODY()
	
public:	
	APortal();

	void DestroyPortal();

	FORCEINLINE TObjectPtr<USceneComponent> GetStartPoint() { return StartPoint; }
	FORCEINLINE TObjectPtr<USceneComponent> GetEndPoint() { return EndPoint; }
	FORCEINLINE TObjectPtr<USceneComponent> GetMovePoint() { return MovePoint; }
	FORCEINLINE TObjectPtr<UParticleSystemComponent> GetBlueAura() { return BlueAura; }
	FORCEINLINE TObjectPtr<UParticleSystemComponent> GetRedAura() { return RedAura; }

	UPROPERTY(VisibleDefaultsOnly, Category = "Teleport")
	TObjectPtr<UMagicTeleportSkill> SkillOwner = nullptr;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Teleport")
	TObjectPtr<USceneComponent> StartPoint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Teleport")
	TObjectPtr<USceneComponent> EndPoint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Teleport")
	TObjectPtr<USceneComponent> MovePoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> BlueAura;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UParticleSystemComponent> RedAura;
};