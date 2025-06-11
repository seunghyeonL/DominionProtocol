#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ItemScanning.generated.h"

class UMagicItemScanningSkill;

UCLASS()
class DOMINIONPROTOCOL_API UAnimNotify_ItemScanning : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

private:
	UPROPERTY()
	TObjectPtr<AActor> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<UMagicItemScanningSkill> ItemScanningSkill;
};
