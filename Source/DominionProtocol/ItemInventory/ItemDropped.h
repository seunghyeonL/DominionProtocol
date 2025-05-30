#pragma once

#include "CoreMinimal.h"
#include "ItemInventory/BaseItem.h"
#include "Particles/ParticleSystemComponent.h"
#include "ItemDropped.generated.h"

UCLASS()
class DOMINIONPROTOCOL_API AItemDropped : public ABaseItem
{
	GENERATED_BODY()

public:
	AItemDropped();

protected:
	// 아이템 주변 아우라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* AuraVFXComponent;

	//얻을 아이템 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ABaseItem> ItemSubclassToAward;

	virtual void BeginPlay() override;

	virtual void Interact_Implementation(AActor* Interactor) override;
};
